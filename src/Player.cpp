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

#include <iostream>
#include <sstream>
#include <fstream>
#include <Player.hpp>
#include <Util.hpp>
#include <GL/freeglut.h>

Player::Player()
{
    loadSkybox("skybox1");
}

Player::~Player()
{
    delete m_skyboxTexture;
    m_skyboxTexture = nullptr;
}

void Player::update(const float dt)
{
    if (m_isGoingForward) {
        if (m_velocity > k_maxVelocity) {
            m_velocity = k_maxVelocity;
        } else {
            m_velocity += k_acceleration * static_cast<double>(dt);
        }
    } else if (isEngineOn()) {
        if (m_velocity > 0) {
            m_velocity -= k_acceleration * static_cast<double>(dt);
        } else {
            m_velocity = 0;
        }
    }

    if (m_isTurningLeft && isEngineOn()) {
        if (m_turnLeftVelocity > k_maxTurnVelocity) {
            m_turnLeftVelocity = k_maxTurnVelocity;
        } else {
            m_turnLeftVelocity += k_turnAcceleration * static_cast<double>(dt);
        }
    } else {
        if (m_turnLeftVelocity > 0) {
            m_turnLeftVelocity -= k_turnAcceleration * static_cast<double>(dt);
        } else {
            m_turnLeftVelocity = 0;
        }
    }

    if (m_isTurningRight && isEngineOn()) {
        if (m_turnRightVelocity > k_maxTurnVelocity) {
            m_turnRightVelocity = k_maxTurnVelocity;
        } else {
            m_turnRightVelocity += k_turnAcceleration * static_cast<double>(dt);
        }
    } else {
        if (m_turnRightVelocity > 0) {
            m_turnRightVelocity -= k_turnAcceleration * static_cast<double>(dt);
        } else {
            m_turnRightVelocity = 0;
        }
    }

    const auto turnFactor = m_velocity / k_maxVelocity;
    const auto rotationFactor = turnFactor * (m_turnLeftVelocity * static_cast<double>(dt) + m_turnRightVelocity * static_cast<double>(-dt));

    m_direction = m_direction.rotated({ 0, rotationFactor, 0 }).normalized();
    m_position += m_direction * m_velocity * dt;

    m_turnRollAngle = 22.5 * turnFactor * (m_turnRightVelocity / k_maxTurnVelocity - m_turnLeftVelocity / k_maxTurnVelocity);
    m_turnYawAngle = 22.5 * turnFactor * (-m_turnRightVelocity / k_maxTurnVelocity + m_turnLeftVelocity / k_maxTurnVelocity);
    m_thrustPitchAngle = 22.5 * turnFactor * (m_velocity / k_maxVelocity);
    m_thrustAdvance = m_velocity / k_maxVelocity;

    m_psi = 90 - Util::rad2deg(atan2(m_direction.z, m_direction.x));

    if (m_fuel > 0)
        m_fuel -= (m_velocity + m_turnLeftVelocity + m_turnRightVelocity) * k_fuelConsumptionUnit;
    else
        m_fuel = 0;

#if 0
    // Active thrust (see below)
    static bool isThrustActive = false;

    // Destabilization
    const float destabilizationFactor = isThrustActive ? 0.0f : 1.0f;
    static int destabilizationIndex = 0;
    m_alpha = 7.5f * destabilizationFactor * sinf(destabilizationIndex++ * Util::k_tau / 450.0f);
    //m_offset.x = 0.25f * sinf(ctr++ * Util::k_tau / 200.0f);
    m_offset.y = 0.05f * destabilizationFactor * sinf(destabilizationIndex++ * Util::k_tau / 200.0f);

#pragma region Initial thrust animation
    static int thrustIndex = 0;
    static bool didPlayThrustAnimation = false;
    if (m_isGoingForward && !didPlayThrustAnimation) {
        // Start initial thrust (W pressed and not played initial thrust animation yet)
        m_offset.z = 0.05 * thrustIndex++ * sinf(destabilizationIndex++ * Util::k_tau / 200.0f);
        if (m_offset.z < 0.15) {
            // Bounce!
            static int bounceCount = 0;
            bounceCount++;
            if (bounceCount == 12) {
                // Enough bouncing -- stop!
                didPlayThrustAnimation = true;
                bounceCount = 0;
            }
        }
    } else if (thrustIndex > 0) {
        // Debounce
        m_offset.z = 0.05 * thrustIndex-- * sinf(destabilizationIndex++ * Util::k_tau / 200.0f);
    }
    // Allow for the animation to replay when pressing W again
    if (didPlayThrustAnimation && !m_isGoingForward)
        didPlayThrustAnimation = false;
#pragma endregion
#pragma region Continued thrust animation
    static double advance = 0.0;

    if (didPlayThrustAnimation) {
        if (m_isGoingForward) {
            // Start thrusting (W pressed and finished playing initial thrust animation)
            isThrustActive = true;
            if (advance < 0.2) {
                advance += 0.015;
                m_pitch += 10 * advance;
                m_offset.z += advance;
            }
        }
    } else if (isThrustActive) {
        // Finished thrusting (W released)
        advance += 0.15;
        m_offset.z -= advance;
        m_pitch -= 5;
        if (m_offset.z < -0.2) {
            isThrustActive = false;
            advance = 0.0;
            m_pitch = 0.0;
            m_offset.z = 0.0;
        }
    }

    Util::consolePrint("dx: %f", destabilizationFactor);
#pragma endregion
#endif
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
    /* if (m_isBirdView) {
        const auto eye = m_up * 50;
        gluLookAt(m_position.x, m_up.y * 50, m_position.z - 10, m_position.x, m_position.y + 2.5, m_position.z, m_up.x, m_up.y, m_up.z);
    } else {
        gluLookAt(m_position.x, m_position.y + 5, m_position.z - 10, m_position.x, m_position.y + 2.5, m_position.z, m_up.x, m_up.y, m_up.z);
    }*/

    if (isEngineOn()) {
        m_cameraPosition = m_position - (m_direction * Vec3<double>{ 10, 10, 10 });
        m_centerPosition = m_position - (m_direction * Vec3<double>{ 0, 0, 0 });
    }

    gluLookAt(
        m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z,
        m_centerPosition.x, m_centerPosition.y, m_centerPosition.z,
        0, 1, 0);
}

void Player::render() const
{
    glPushMatrix();
    {
        // Skybox matrix
        glTranslated(m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z);
        m_skyboxTexture->bind();
        m_skybox.render(50.0f);
        glPopMatrix();
    }

    glPushMatrix();
    {
        // Player matrix
        glTranslated(m_position.x, m_position.y, m_position.z);
        glRotated(m_psi, 0, 1, 0);

        const auto thrustAdvance = m_direction * m_thrustAdvance;
        glTranslated(thrustAdvance.x, thrustAdvance.y, thrustAdvance.z);
        glRotated(m_thrustPitchAngle, 1, 0, 0);
        glRotated(m_turnYawAngle, 0, 1, 0);
        glRotated(m_turnRollAngle, 0, 0, 1);

        m_model.render(0.001f);
        glPopMatrix();
    }

    // Fog
    if (m_isFogEnabled) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP);
        glFogfv(GL_FOG_COLOR, m_fogColor);
        glFogf(GL_FOG_DENSITY, 0.025f);
    } else {
        glDisable(GL_FOG);
    }

    glPushMatrix();
    {
        // Test cube matrix
        static Model testCube{ "data/TEST_CUBE/TEST_CUBE.obj" };
        glTranslated(2, 0, 4);
        testCube.render(0.5f);
        glPopMatrix();
    }

    // Draw direction vector
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3d(m_direction.x, m_direction.y, m_direction.z);
    glVertex3d(10 * m_direction.x, 10 * m_direction.y, 10 * m_direction.z);
    glEnd();
    glPopMatrix();

#if 0
    glPopMatrix();
#endif

    Util::consolePrint("SHIP");
    Util::consolePrint("  fuel: %12lf", m_fuel);

    Util::consolePrint("PLAYER");
    Util::consolePrint("  ctl: %c%c%c", m_isGoingForward ? 'F' : '-', m_isTurningLeft ? 'L' : '-', m_isTurningRight ? 'R' : '-');
    Util::consolePrint("  pos: %12lf %12lf %12lf", m_position.x, m_position.y, m_position.z);
    Util::consolePrint("  dir: %12lf %12lf %12lf", m_direction.x, m_direction.y, m_direction.z);
    Util::consolePrint("  vel: %12lf %12lf %12lf", m_velocity, m_turnLeftVelocity, m_turnRightVelocity);

    Util::consolePrint("DEBUG");
    Util::consolePrint("  view:  %s", m_isBirdView ? "BRD" : "PRP");
    Util::consolePrint("  fog:   %s", m_isFogEnabled ? "YES" : "NO");
    Util::consolePrint("  accum: %s", m_isMotionBlurEnabled ? "YES" : "NO");
    Util::consolePrint("  msaa:  %s", m_isMsaaEnabled ? "YES" : "NO");
}

void Player::handleKeyboardEvent(const int up, const unsigned char key, int x, int y)
{
    // Player/camera controls
    if (key == 'w')
        m_isGoingForward = !static_cast<bool>(up);
    if (key == 'a')
        m_isTurningLeft = !static_cast<bool>(up);
    if (key == 'd')
        m_isTurningRight = !static_cast<bool>(up);

    // Option toggles
    if (key == 'b' && up)
        m_isBirdView = !m_isBirdView;
    if (key == 'f' && up)
        m_isFogEnabled = !m_isFogEnabled;
    if (key == 's' && up)
        m_isSkyboxVisible = !m_isSkyboxVisible;
    if (key == 'c' && up)
        m_isConsoleVisible = !m_isConsoleVisible;
    if (key == 'm' && up)
        m_isMotionBlurEnabled = !m_isMotionBlurEnabled;
    if (key == 'n' && up)
        m_isMsaaEnabled = !m_isMsaaEnabled;
}

void Player::loadSkybox(const std::string& name)
{
    // Read ambient/fog color
    std::ifstream ambientFileStream("data/skybox/" + name + "_ambient.dat");
    if (std::string line; std::getline(ambientFileStream, line)) {
        std::istringstream ambientColorStream(line);

        // Read R, G, B
        for (int i = 0; i < 3; i++) {
            std::string channel;
            assert(ambientColorStream >> channel);
            m_fogColor[i] = static_cast<float>(std::stoi(channel)) / 255.0f;
            m_skyboxColor[i] = m_fogColor[i] + 0.5f;
        }

        m_fogColor[3] = 1.0f;
        m_skyboxColor[3] = 1.0f;

        // Set ambient light
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, m_skyboxColor);
        glEnable(GL_LIGHTING);
    }

    // Read texture
    m_skyboxTexture = new Texture("data/skybox/" + name + ".png");
}
