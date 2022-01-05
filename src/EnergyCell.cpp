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

#include <EnergyCell.hpp>

Model* EnergyCell::s_boxModel = nullptr;

EnergyCell::EnergyCell(const Vec3<float>& position)
    : m_position(position)
{
}

void EnergyCell::update(float dt, const Player& player)
{
    m_keyframe = (m_keyframe + static_cast<int>(350 * dt)) % 360;

    if (const auto distance = (m_position - player.getPosition()).magnitude();
        distance < 5) {
        // Player collided with this energy cell
        m_hasCollidedWithPlayer = true;
        if (m_callback)
            m_callback(*this);
    }
}

void EnergyCell::render() const
{
    glPushMatrix();
    {
        glTranslatef(m_position.x, m_position.y, m_position.z);
        glRotatef(m_keyframe, 0, 1, 1);
        getModel().render();
        glPopMatrix();
    }
}