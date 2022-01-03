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

#include <Tunnel.hpp>

Tunnel::Tunnel()
{
}

void Tunnel::update(float dt, Player& player)
{
    Util::consolePrint("ENDD %f", (player.getPosition() - trajectory(m_endChunk)).magnitude());

    m_nearestChunk = chunkFromPosition(player.getPosition());
    m_distanceFromNearestChunk = (player.getPosition() - trajectory(m_nearestChunk)).magnitude();

    if (const auto distanceFromEnd = (player.getPosition() - trajectory(m_endChunk)).magnitude();
        distanceFromEnd < k_minGenDistance) {
        // Player is approaching the end of the tunnel
        m_endChunk++; // Render more chunks

        if (m_endChunk % k_energyCellPeriod == 0) {
            // Spawn a new energy cell
            m_energyCells.push_back(EnergyCell(trajectory(m_endChunk)));
            m_energyCells[m_energyCells.size() - 1].bindCollisionCallback([&player](const auto&) {
                player.rechargeFuel();
            });
        }

        if ((m_endChunk - m_startChunk) > k_maxChunks)
            m_startChunk++;
    }

    if (const auto distanceFromStart = (player.getPosition() - trajectory(m_startChunk)).magnitude();
        distanceFromStart < k_minGenDistance && (m_endChunk - m_startChunk) > k_minChunks) {
        // Player is approaching the start of the tunnel
        m_endChunk--; // Render more chunks
    }

    for (auto& energyCell : m_energyCells)
        energyCell.update(dt, player);
}

void Tunnel::render() const
{
    for (unsigned int chunk = m_startChunk; chunk <= m_endChunk; chunk++) {

    }

#if 0
    for (unsigned int chunk = m_startChunk; chunk <= m_endChunk; chunk++) {
        const auto position = trajectory(chunk);

        glColor3f(1, m_nearestChunk == chunk ? (1 - (m_distanceFromNearestChunk / 50)) : 0, 0);

        glPushMatrix();
        glTranslatef(position.x, position.y + 1, position.z);
        glutSolidCube(0.5);
        glPopMatrix();
    }
#endif

    for (const auto& energyCell : m_energyCells)
        energyCell.render();
}
