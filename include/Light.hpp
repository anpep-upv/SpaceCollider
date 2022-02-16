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

#include <Model.hpp>
#include <Vec3.hpp>

struct Light : Model {
    explicit Light(const Vec3<float>& position, const Vec3<float>& direction, const Player& player)
        : Model("data/light_floor/light_floor.obj", position, Vec3(0.25f), 270, Vec3(1.0f, 0.0f, 0.0f))
        , m_directionYaw(90 - atan2(direction.z, direction.x) * 180 / Util::k_pi)
        , m_player(player)
    {
    }

    bool isLightEnabled() const { return m_isLightEnabled; }

    void update(float dt) override
    {
        m_distanceFromPlayer = (m_player.getPosition() - m_position).magnitude();
        m_isLightEnabled = m_distanceFromPlayer < 50;
    }

    void render() const override
    {
#ifndef __APPLE__
        glPushMatrix();
        {
            glTranslatef(m_position.x, m_position.y, m_position.z);
            glScalef(m_scale.x, m_scale.y, m_scale.z);
            glRotatef(m_rotationAngle, m_rotationAxis.x, m_rotationAxis.y, m_rotationAxis.z);
            glRotatef(m_directionYaw, 0, 0, 1);
            renderNoTransform();
            glPopMatrix();
        }

        if (m_isLightEnabled) {

            const float lightPosition[4] { m_position.x, m_player.getPosition().y + 2, m_position.z, 1 };
            glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

            const float lightColor[4] { 1, 1, 1, 1 };
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);

            const float specularColor[4] { 1, 1, 1, 1 };
            glLightfv(GL_LIGHT1, GL_SPECULAR, specularColor);

            const float spotDirection[3] { 0, -1, 0 };
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);

            float x = pow(Util::k_e, m_distanceFromPlayer / 10);
            glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, x);
            glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, x);
            glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, x);

            glEnable(GL_LIGHT1);
        }
#endif
    }

private:
    const float m_directionYaw;
    const Player& m_player;

    float m_distanceFromPlayer;
    bool m_isLightEnabled;
};