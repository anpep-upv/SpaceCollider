// SpaceCollider -- SGI Lab Task 9
// Copyright (c) 2021-2022 �ngel P�rez <aperpor@upv.edu.es>
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

#include <Vec3.hpp>
#include <Model.hpp>

struct Player {
    Player();
    ~Player();

#pragma region Rendering and control
    // Update object properties
    void update(float dt);

    // Update camera view
    void updateCamera();

    // Respond to changes in the viewport
    static void updateViewport(int width, int height);

    // Renders the player and skybox
    void render() const;

    // Handles OpenGL keyboard events
    void handleKeyboardEvent(int up, unsigned char key, int x, int y);
#pragma endregion

#pragma region Player properties
    // Do we have fuel?
    bool isEngineOn() const
    {
        return m_fuel > std::numeric_limits<double>::epsilon();
    }
#pragma endregion

#pragma region Debug switch getters
    bool isBirdView() const { return m_isBirdView; }
    bool isFogEnabled() const { return m_isFogEnabled; }
    bool isSkyboxVisible() const { return m_isSkyboxVisible; }
    bool isConsoleVisible() const { return m_isConsoleVisible; }
    bool isMotionBlurEnabled() const { return m_isMotionBlurEnabled; }
    bool isMsaaEnabled() const { return m_isMsaaEnabled; }
#pragma endregion

private:
    void loadSkybox(const std::string& name);

#pragma region Skybox
    Texture* m_skyboxTexture = nullptr;
    Model m_skybox{ "data/skybox/skybox.obj" };

    float m_fogColor[4]{};
    float m_skyboxColor[4]{};
#pragma endregion

#pragma region Camera
    static constexpr double k_fov = 45.0;
    static constexpr double k_near = 0.5;
    static constexpr double k_far = 100.0;

    Vec3<double> m_cameraPosition;
    Vec3<double> m_centerPosition;

    double m_psi = 0.0;
#pragma endregion

#pragma region Physics
    static constexpr double k_acceleration = 5.0;
    static constexpr double k_turnAcceleration = 5.0;

    static constexpr double k_maxVelocity = 10.0;
    static constexpr double k_maxTurnVelocity = 2.0;

    double m_velocity = 0.0;
    double m_turnLeftVelocity = 0.0;
    double m_turnRightVelocity = 0.0;

    // Ship roll due to turning
    double m_turnRollAngle = 0.0;
    // Ship yaw due to turning
    double m_turnYawAngle = 0.0;
    // Ship pitch due to thrust
    double m_thrustPitchAngle = 0.0;
    // Ship advance due to thrust
    double m_thrustAdvance = 0.0;
#pragma endregion

#pragma region Ship properties
    static constexpr double k_fuelConsumptionUnit = 0.0000125;

    // Model object
    Model m_model{ "data/PLAHB1/PLAHB1.obj" };

    // Current object position in world
    Vec3<double> m_position{ 0, 0, 0 };
    // Current object direction
    Vec3<double> m_direction{ 0, 0, 1 };

    // Current fuel
    double m_fuel = 1.0;
#pragma endregion

#pragma region Debug switches
    bool m_isBirdView{ false };
    bool m_isFogEnabled{ false };
    bool m_isSkyboxVisible{ true };
    bool m_isConsoleVisible{ true };
    bool m_isMotionBlurEnabled{ false };
    bool m_isMsaaEnabled{ true };
#pragma endregion

#pragma region Control
    bool m_isGoingForward{ false };
    bool m_isTurningLeft{ false };
    bool m_isTurningRight{ false };
#pragma endregion
};
