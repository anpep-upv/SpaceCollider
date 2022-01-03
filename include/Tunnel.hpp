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

#include <EnergyCell.hpp>
#include <Player.hpp>
#include <Vec3.hpp>

struct Tunnel {
    Tunnel();

    void update(float dt, Player& player);
    void render() const;

private:
    void renderChunk(unsigned int chunk) const;

    // Minimum distance from the player to the last chunk of the tunnel that will be
    // visible before starting to generate new chunks
    static constexpr float k_minGenDistance = 500;

    // Minimum number of chunks that can be rendered at a given time
    static constexpr unsigned int k_minChunks = 100;

    // Maximum number of chunks that can be rendered at a given time
    static constexpr unsigned int k_maxChunks = 250;

    // Length of each chunk
    static constexpr float k_chunkLength = 5;

    // Spawn an energy cell every n chunks
    static constexpr unsigned int k_energyCellPeriod = 30;

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
    unsigned int m_endChunk = m_startChunk; //m_startChunk + k_minChunks;

    unsigned int m_nearestChunk = m_startChunk;
    float m_distanceFromNearestChunk = 0;

    std::vector<EnergyCell> m_energyCells;
};
