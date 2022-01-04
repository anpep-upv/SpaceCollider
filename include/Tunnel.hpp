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

#include <cmath>
#include <vector>

#include <BeaconModel.hpp>
#include <EnergyCell.hpp>
#include <Light.hpp>
#include <Player.hpp>
#include <Vec3.hpp>

struct Tunnel {
    void update(float dt, Player& player);
    void render() const;

private:
    // Renders the light from beneath the tunnel
    void renderBottomLight() const;

    // Minimum distance from the player to the last chunk of the tunnel that will be
    // visible before starting to generate new chunks
    static constexpr float k_minGenDistance = 1500;

    // Minimum number of chunks that can be rendered at a given time
    static constexpr unsigned int k_minChunks = 750;

    // Maximum number of chunks that can be rendered at a given time
    static constexpr unsigned int k_maxChunks = 1500;

    // Length of each chunk
    static constexpr float k_chunkLength = 5;

    // Spawn an energy cell every n chunks
    static constexpr unsigned int k_energyCellPeriod = 40;

    // Spawn a beacon every n chunks
    static constexpr unsigned int k_beaconPeriod = 20;

    // Spawn a light every n chunks
    static constexpr unsigned int k_lightPeriod = 25;

    // Number of light threads
    static constexpr unsigned int k_lightThreads = 48;

    // Distance of every light thread from the center of the tunnel
    static constexpr unsigned int k_tunnelRadius = 16;

    inline Vec3<float> trajectory(unsigned int chunk) const
    {
        constexpr float amplitude = 250, period = 250;
        return { amplitude * std::sin(Util::k_pi * chunk / period), 0, k_chunkLength * chunk };
    }

    inline unsigned int chunkFromPosition(const Vec3<float>& position)
    {
        return std::ceil(position.z / k_chunkLength);
    }

    unsigned int m_startChunk = 1;
    unsigned int m_endChunk = 1;

    unsigned int m_nearestChunk = m_startChunk;
    float m_distanceFromNearestChunk = 0;

    Vec3<float> m_playerPosition;

    std::vector<std::unique_ptr<EnergyCell>> m_energyCells;
    std::vector<std::unique_ptr<BeaconModel>> m_beacons;
    std::vector<std::unique_ptr<Light>> m_lights;
};
