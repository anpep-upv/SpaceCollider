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

#include <Beacon.hpp>

std::shared_ptr<Model> Beacon::s_beaconModel = std::make_shared<Model>("data/TBEACR/TBEACR.obj", Vec3(), Vec3(0.001f));

Beacon::Beacon(const Vec3<float>& position)
    : m_position(position)
{
}

void Beacon::render() const
{
    glPushMatrix();
    {
        glTranslatef(m_position.x, m_position.y, m_position.z);
        s_beaconModel->render();
        glPopMatrix();
    }
}