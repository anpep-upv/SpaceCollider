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

#include <cmath>
#include <HUD.hpp>
#include <Model.hpp>
#include <Vec3.hpp>

struct Player {
    void update(float dt);
    void updateCamera();
    static void updateViewport(int width, int height);

    void render() const;
    void handleKeyboardEvent(int up, unsigned char key, int x, int y);

    bool isEngineOn() const { return m_fuel > 0; }

    const Vec3<float>& getPosition() const { return m_position; }

    void rechargeFuel()
    {
        if (isEngineOn())
            m_fuel = std::min(1.0, m_fuel + 0.75);
    }

private:
    HUD m_hud;

    // Camera constants
    static constexpr double k_fov = 45.0;
    static constexpr double k_near = 0.5;
    static constexpr double k_far = 2500.0;

    // Camera properties
    Vec3<float> m_cameraPosition;
    Vec3<float> m_centerPosition;

    // Physics constants
    static constexpr float k_acceleration = 10.0f;
    static constexpr float k_turnAcceleration = 5.0f;
    static constexpr float k_maxVelocity = 50.0f;
    static constexpr float k_maxTurnVelocity = 2.0f;
    static constexpr float k_fuelConsumptionUnit = 0.00005f;

    // Physics properties
    float m_velocity = 0.0f;
    float m_turnLeftVelocity = 0.0f;
    float m_turnRightVelocity = 0.0f;

    // Current object position in world
    Vec3<float> m_position;
    // Current object direction
    Vec3<float> m_direction { 0, 0, 1 };

    // Current fuel
    float m_fuel = 1.0f;

    // Ship roll due to turning
    float m_turnRollAngle = 0.0f;
    // Ship yaw due to turning
    float m_turnYawAngle = 0.0f;
    // Ship pitch due to thrust
    float m_thrustPitchAngle = 0.0f;
    // Ship advance due to thrust
    float m_thrustAdvance = 0.0f;
    // Ship yaw associated to general direction
    float m_directionYaw = 0.0f;

    // Physics toggles
    bool m_isThrusting = false;
    bool m_isBraking = false;
    bool m_isTurningLeft = false;
    bool m_isTurningRight = false;

    // Debug/camera toggles
    bool m_isHUDVisible = true;
    bool m_isBirdViewActive = false;

    // Player ship model
    Model m_model { "data/PLAHB1/PLAHB1.obj", {}, Vec3(0.001f) };
};
