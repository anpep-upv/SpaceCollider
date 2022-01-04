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

#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct SoundtrackManager {
    static SoundtrackManager& the()
    {
        if (!s_instance)
            s_instance = new SoundtrackManager;
        return *s_instance;
    }

    ~SoundtrackManager();

    bool isTrackLoaded() const { return m_trackStream.good(); }
    bool isTrackPlaying() const { return m_isTrackPlaying; }
    const std::string& getTrackName() const { return m_trackName; }

    bool enqueueTrack(const std::string& path);
    bool playNextTrack();
    bool stopTrack();

    bool skipNextTrack();
    bool skipPreviousTrack();

private:
    SoundtrackManager() { }

    bool loadTrack(const std::string& path);
    bool threadPlayTrack();

    static SoundtrackManager* s_instance;

    std::thread m_playbackThread;
    std::atomic<bool> m_isTrackPlaying;
    std::mutex m_trackStreamLock;
    std::ifstream m_trackStream;

    std::string m_trackPath;
    std::string m_trackName;

    std::vector<std::string> m_trackQueue;
    unsigned int m_trackQueueIndex;
};