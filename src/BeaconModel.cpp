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

#include <BeaconModel.hpp>

LightId BeaconModel::s_lastId = GL_LIGHT1;

BeaconModel::BeaconModel(const Vec3<float>& position)
    : m_model("data/TBEACR/TBEACR.obj", position, Vec3(0.001f), 90, Vec3(0.0f, 1.0f, 0.0f))
{
    m_id = s_lastId;
    s_lastId = GL_LIGHT1 + (s_lastId - GL_LIGHT1 + 1) % 8;
}

BeaconModel::~BeaconModel()
{
    glDisable(m_id);
}

void BeaconModel::update(float dt)
{
}

void BeaconModel::render() const
{
    Util::consolePrint("GL_LIGHT%u", m_id - GL_LIGHT0);

    const float lightAmbient[4] { 0, 0, 0, 0 };
    glLightfv(m_id, GL_AMBIENT, lightAmbient);

    const float lightPosition[4] { m_model.getPosition().x, m_model.getPosition().y + 1, m_model.getPosition().z, 1 };
    glLightfv(m_id, GL_POSITION, lightPosition);

    const float lightColor[4] { 0, 0, 0, 1 };
    glLightfv(m_id, GL_DIFFUSE, lightColor);

    const float spotDirection[3] { 0, -1, 0 };
    glLightf(m_id, GL_SPOT_CUTOFF, 90);
    glLightfv(m_id, GL_SPOT_DIRECTION, spotDirection);
    glLightf(m_id, GL_SPOT_EXPONENT, 5);

    glEnable(m_id);
    m_model.render();
}