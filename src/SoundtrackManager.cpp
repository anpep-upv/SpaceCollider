// SpaceCollider -- SGI Lab Task 9
// Copyright (c) 2021-2022 Ángel Pérez <aperpor@upv.edu.es>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <filesystem>
#include <fstream>

#ifdef __linux__
#include <memory>

#include <pulse/simple.h>

struct PASimpleDeleter {
    void operator()(pa_simple* streamPtr) const
    {
        // FIXME: is it okay to pass nullptr to error here?
        pa_simple_drain(streamPtr, nullptr);
    }
};
using PASimpleWrapper = std::unique_ptr<pa_simple, PASimpleDeleter>;
#elif defined(_WIN32)
#endif

#include <SoundtrackManager.hpp>

SoundtrackManager* SoundtrackManager::s_instance;

SoundtrackManager::~SoundtrackManager()
{
    stopTrack();
    if (m_playbackThread.joinable())
        m_playbackThread.join();
}

bool SoundtrackManager::enqueueTrack(const std::string& path)
{
    m_trackQueue.push_back(path);
    return true;
}

bool SoundtrackManager::loadTrack(const std::string& path)
{
    const std::filesystem::path fsPath(path);
    if (m_trackStream.is_open())
        m_trackStream.close();
    m_trackStream.open(path, std::ifstream::binary);
    m_trackName = fsPath.stem().string();
    return !m_trackStream.fail();
}

bool SoundtrackManager::playNextTrack()
{
    if (isTrackPlaying())
        return false;
    if (m_playbackThread.joinable())
        m_playbackThread.join();
    m_playbackThread = std::thread(&SoundtrackManager::threadPlayTrack, this);
    return true;
}

bool SoundtrackManager::stopTrack()
{
    if (!isTrackPlaying())
        return false;
    m_isTrackPlaying = false;
    if (m_playbackThread.joinable())
        m_playbackThread.join();
    return true;
}

bool SoundtrackManager::threadPlayTrack()
{
#ifdef __linux__
    const pa_sample_spec sampleSpec {
        // These are fixed! We converted the soundtrack to single-channel 8KHz PCM U8
        // with the following command:
        //   ffmpeg -i soundtrack.mp3 -ac 1 -ar 8000 -acodec pcm_u8 soundtrack.wav
        .format = PA_SAMPLE_U8,
        .rate = 8000,
        .channels = 1
    };

    // Create playback stream
    const PASimpleWrapper stream(pa_simple_new(
        nullptr, // Server name (default)
        "SpaceCollider", // Client name
        PA_STREAM_PLAYBACK, // Stream direction (playback)
        nullptr, // Default device
        m_trackName.c_str(), // Track name
        &sampleSpec, // Sample type to use
        nullptr, // Default channel map
        nullptr, // Default buffering attributes
        nullptr // Don't store error code
        ));
    char buffer[1024];

    m_trackStreamLock.lock();
    m_isTrackPlaying = true;
    while (m_isTrackPlaying) {
        // Ignore WAV header
        m_trackStream.ignore(44);
        while (m_isTrackPlaying && m_trackStream.read(buffer, sizeof(buffer)))
            pa_simple_write(stream.get(), buffer, sizeof(buffer), nullptr);

        if (!m_trackQueue.empty()) {
            // Load next track
            m_trackQueueIndex = (m_trackQueueIndex + 1) % m_trackQueue.size();
            loadTrack(m_trackQueue[m_trackQueueIndex]);
        } else {
            // Reset stream and loop
            m_trackStream.clear();
            m_trackStream.seekg(0, std::ios::beg);
        }
    }
    m_trackStreamLock.unlock();

    return true;
#elif defined(_WIN32)
#endif
    return false;
}
