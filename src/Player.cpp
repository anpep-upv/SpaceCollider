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

#include <cmath>

#include <GL/freeglut.h>
#include <Keymap.hpp>
#include <Player.hpp>
#include <Util.hpp>

void Player::update(const float dt)
{
    if (m_isThrusting) {
        if (m_velocity > k_maxVelocity)
            m_velocity = k_maxVelocity;
        else
            m_velocity += k_acceleration * dt;
    } else if (isEngineOn()) {
        if (m_velocity > 0)
            m_velocity -= k_acceleration * dt;
        else
            m_velocity = 0;
    }

    if (isEngineOn() && m_isBraking) {
        if (m_velocity > 0)
            m_velocity -= 5 * k_acceleration * dt;
        else
            m_velocity = 0;
    }

    if (m_isTurningLeft && isEngineOn()) {
        if (m_turnLeftVelocity > k_maxTurnVelocity)
            m_turnLeftVelocity = k_maxTurnVelocity;
        else
            m_turnLeftVelocity += k_turnAcceleration * dt;
    } else {
        if (m_turnLeftVelocity > 0)
            m_turnLeftVelocity -= k_turnAcceleration * dt;
        else
            m_turnLeftVelocity = 0;
    }

    if (m_isTurningRight && isEngineOn()) {
        if (m_turnRightVelocity > k_maxTurnVelocity)
            m_turnRightVelocity = k_maxTurnVelocity;
        else
            m_turnRightVelocity += k_turnAcceleration * dt;
    } else {
        if (m_turnRightVelocity > 0)
            m_turnRightVelocity -= k_turnAcceleration * dt;
        else
            m_turnRightVelocity = 0;
    }

    // Advance position
    m_position += m_direction * m_velocity * dt;

    if (isEngineOn()) {
        // Limit left/right turn by forward velocity
        const auto turnFactor = m_velocity / k_maxVelocity;
        // Limit rotation by left/right turn velocity
        const auto rotationFactor = turnFactor * (m_turnLeftVelocity * dt + m_turnRightVelocity * -dt);

        // Rotate general ship direction
        m_direction = m_direction.rotated({ 0, rotationFactor, 0 }).normalized();
        // Set roll angle due to left/right turn
        m_turnRollAngle = 22.5 * turnFactor * (m_turnRightVelocity / k_maxTurnVelocity - m_turnLeftVelocity / k_maxTurnVelocity);
        // Set yaw angle due to left/right turn
        m_turnYawAngle = 22.5 * turnFactor * (-m_turnRightVelocity / k_maxTurnVelocity + m_turnLeftVelocity / k_maxTurnVelocity);
        // Set pitch angle (back bump up) due to thrust
        m_thrustPitchAngle = 22.5 * turnFactor * (m_velocity / k_maxVelocity);
        // Set slight offset in advance direction due to thrust
        m_thrustAdvance = m_velocity / k_maxVelocity;

        // Recalculate yaw in general direction
        m_directionYaw = 90 - atan2f(m_direction.z, m_direction.x) * 180.0f / Util::k_pi;
        // Consume fuel (forward thrusters + left/right turn thrusters)
        // m_fuel -= (m_velocity + m_turnLeftVelocity + m_turnRightVelocity) * k_fuelConsumptionUnit;
    } else {
        // Engine is off! Spin endlessly due to the abscence of gravity
        m_thrustPitchAngle += m_velocity * dt;
        m_fuel = 0;
    }

    m_hud.updateFuel(m_fuel, 1.0f);
    m_hud.updateSpeed(m_velocity, k_maxVelocity);

    const float relativeVelocity = m_velocity / k_maxVelocity;
    auto &turnedOffThrusterMaterial = m_model.getMaterialCollection()->getMaterial("tex_13"),
         &turnedOnThrusterMaterial = m_model.getMaterialCollection()->getMaterial("tex_13_additive");
    
    turnedOnThrusterMaterial.isVisible = m_isThrusting;
    turnedOffThrusterMaterial.isVisible = !m_isThrusting;

    if (turnedOnThrusterMaterial.isVisible) {
        for (int i = 0; i < 3; i++)
            turnedOnThrusterMaterial.emissiveReflectivity[i] = relativeVelocity;
    }
}

void Player::updateViewport(const int width, const int height)
{
    const auto aspectRatio = static_cast<double>(width) / static_cast<double>(height);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(k_fov, aspectRatio, k_near, k_far);
}

void Player::updateCamera()
{
    if (isEngineOn()) {
        // Camera stops when engine is stopped (so that the ship just flies away slowly)
        if (m_isBirdViewActive) {
            m_cameraPosition = m_position + Vec3<float> { 0, 50, 50 };
            m_centerPosition = m_position;
        } else {
            m_cameraPosition = m_position - (m_direction * Vec3<float> { 10, 10, 10 });
            m_centerPosition = m_position - (m_direction * Vec3<float> { 0, 0, 0 });
        }
    }

    gluLookAt(m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z, m_centerPosition.x, m_centerPosition.y, m_centerPosition.z, 0, 1, 0);
}

void Player::render() const
{

#if 0
    glPushMatrix();
    {
        // GNN matrix
        static Model cameraModel { "data/MEDISM/MEDISM.obj" };
        static int offsetYCounter = 0;
        offsetYCounter++;
        const Vec3<double> shipPosition { m_position.x, m_position.y + 10, m_position.z + 40.0 };
        glTranslatef(shipPosition.x, shipPosition.y + 0.25 * sin(static_cast<double>(offsetYCounter) / 25.0), shipPosition.z);
        glRotatef(180, 0, 1, 0);
        glRotatef(20, 1, 0, 1);

        cameraModel.render(0.001f);

        static bool cameraIndicatorVisible = true;
        static int cameraIndicatorFrame = 0;
        cameraIndicatorFrame++;

        if (cameraIndicatorFrame % 50 == 0)
            cameraIndicatorVisible = !cameraIndicatorVisible;

        if (cameraIndicatorVisible) {
            glPushMatrix();
            {
                static float color[4] { 1, 0, 0, 1 };
                glMaterialfv(GL_FRONT, GL_AMBIENT, color);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
                glMaterialfv(GL_FRONT, GL_EMISSION, color);

                glTranslatef(-0.25, 2.75, 0.5);
                glutSolidSphere(0.125, 4, 4);
                glPopMatrix();
            }
        }

        glPopMatrix();
    }
#endif

    glPushMatrix();
    {
        // Player matrix
        glTranslatef(m_position.x, m_position.y, m_position.z);
        glRotatef(m_directionYaw, 0, 1, 0);

        const auto thrustAdvance = m_direction * m_thrustAdvance;
        glTranslatef(thrustAdvance.x, thrustAdvance.y, thrustAdvance.z);
        glRotatef(m_thrustPitchAngle, 1, 0, 0);
        glRotatef(m_turnYawAngle, 0, 1, 0);
        glRotatef(m_turnRollAngle, 0, 0, 1);

        m_model.render();
        glPopMatrix();
    }

    if (m_isHUDVisible) {
        glPushMatrix();
        {
            // HUD matrix
            glTranslatef(m_position.x, m_position.y, m_position.z);
            glRotatef(m_directionYaw, 0, 1, 0);

            const auto thrustAdvance = m_direction * m_thrustAdvance;
            glTranslatef(thrustAdvance.x, thrustAdvance.y, thrustAdvance.z);
            glRotatef(m_thrustPitchAngle, 1, 0, 0);
            glRotatef(m_turnYawAngle, 0, 1, 0);
            glRotatef(m_turnRollAngle, 0, 0, 1);
            glTranslatef(-3, 1, 0);
            m_hud.render();
            glPopMatrix();
        }
    }

#if 0
    // Fog
    if (m_isFogEnabled) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogi(GL_FOG_START, k_near);
        glFogi(GL_FOG_END, 0.75 * k_far);
        glFogfv(GL_FOG_COLOR, m_fogColor);
        glFogf(GL_FOG_DENSITY, 0.00075f);
    } else {
        glDisable(GL_FOG);
    }
#endif

    Util::consolePrint("PROP");
    Util::consolePrint("  \5CMR\1  %s", m_isBirdViewActive ? "BRD" : "PRP");
    Util::consolePrint("  \5FUEL\1 %12lf", m_fuel);

    Util::consolePrint("PHYS");
    Util::consolePrint("  \5CTL\1  %cT%cB%cL%cR\1", m_isThrusting ? '\4' : '\5', m_isBraking ? '\4' : '\5', m_isTurningLeft ? '\4' : '\5', m_isTurningRight ? '\4' : '\5');
    Util::consolePrint("  \5POS\1  %12f %12f %12f", m_position.x, m_position.y, m_position.z);
    Util::consolePrint("  \5DIR\1  %12f %12f %12f", m_direction.x, m_direction.y, m_direction.z);
    Util::consolePrint("  \5VEL\1  %12f %12f %12f", m_velocity, m_turnLeftVelocity, m_turnRightVelocity);
}

void Player::handleKeyboardEvent(const int up, const unsigned char key, int, int)
{
    if (!Keymap::the().contains(key))
        return;

    const auto cmd = Keymap::the().at(key);
    switch (cmd) {
        // Player control
    case Keymap::InputCommand::Thrust:
        m_isThrusting = !up;
        break;
    case Keymap::InputCommand::Brake:
        m_isBraking = !up;
        break;
    case Keymap::InputCommand::TurnLeft:
        m_isTurningLeft = !up;
        break;
    case Keymap::InputCommand::TurnRight:
        m_isTurningRight = !up;
        break;

        // Debug toggles
    case Keymap::InputCommand::ToggleHUD:
        if (up)
            m_isHUDVisible = !m_isHUDVisible;
        break;

        // Camera control
    case Keymap::InputCommand::ToggleBirdView:
        if (up)
            m_isBirdViewActive = !m_isBirdViewActive;
        break;
    }
}