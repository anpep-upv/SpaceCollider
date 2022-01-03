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

EnergyCell::EnergyCell(const Vec3<float>& position)
    : m_boxModel("data/box/box2.obj", position, Vec3(1.5f))
{
}

void EnergyCell::update(float dt, const Player& player)
{
    m_keyframe = (m_keyframe + static_cast<int>(350 * dt)) % 360;
    m_boxModel.setRotation(m_keyframe, Vec3(0.0f, 1.0f, 1.0f));

    if (const auto distance = (m_boxModel.getPosition() - player.getPosition()).magnitude();
        m_callback && distance < 5) {
        // Player collided with this energy cell
        m_callback(*this);
    }
}

void EnergyCell::render() const
{
    m_boxModel.render();
}