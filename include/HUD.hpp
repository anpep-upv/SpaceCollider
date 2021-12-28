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

#include <string>

struct HUD {
    HUD();
    ~HUD() = default;

    void render() const;

    void updateSpeed(float currentSpeed, float maxSpeed) { m_speed = static_cast<double>(currentSpeed / maxSpeed); }
    void updateFuel(float currentFuel, float maxFuel) { m_fuel = static_cast<double>(currentFuel / maxFuel); }

private:
    static void renderBar(unsigned int index, const std::string& label, double value);

    double m_speed, m_fuel;
};