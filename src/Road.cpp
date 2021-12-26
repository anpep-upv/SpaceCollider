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

#include "Road.hpp"

Road::Road()
{
    for (int i = 0; i < 250; i++)
        pushSlab();
}

Road::~Road() { }

void Road::update(float dt, const Vec3<double>& player)
{
    const auto& lastSlab = m_slabs.at(m_slabs.size() - 1);
    const auto distanceFromPlayer = sqrtf(powf(lastSlab.x - player.x, 2.0f) + powf(lastSlab.y - player.y, 2.0f) + powf(lastSlab.z - player.z, 2.0f));

    if (distanceFromPlayer < 250) {
        pushSlab();
        popSlab();
    }

    Util::consolePrint("DST    %f", distanceFromPlayer);
}

void Road::render() const
{
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_zero);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_slabColor);
    glMaterialfv(GL_FRONT, GL_EMISSION, m_slabColorEmission);

    for (const auto& slab : m_slabs) {

        glPushMatrix();
        {
            glTranslatef(slab.x, slab.y, slab.z);
            glRotatef(3 * slab.w, 0.0f, 1.0f, 0.0f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);
            glScalef(3.0f, 3.0f, 3.0f);
            glRectf(0.0f, 0.0f, 1.0f, 1.0f);
            glRectf(1.1f, 0.0f, 2.1f, 1.0f);
            glRectf(2.2f, 0.0f, 3.3f, 1.0f);

            glPopMatrix();
        }
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glPopAttrib();
}

void Road::pushSlab()
{
    static int n = 0;
    auto position = m_slabs.empty() ? Vec4() : m_slabs.at(m_slabs.size() - 1);

    position.x = Util::k_tau * 200 / 50 * sinf(static_cast<float>(n) * Util::k_tau / 50);
    position.y = 0;
    position.z += 5;
    position.w = Util::k_tau * 200 / 50 * cosf(static_cast<float>(n) * Util::k_tau / 50);
    

    m_slabs.push_back(position);
    n++;
}

void Road::popSlab()
{
    if (m_slabs.empty())
        return;
    m_slabs.erase(m_slabs.begin());
}
