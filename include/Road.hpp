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

#include <Vec2.hpp>
#include <Vec3.hpp>
#include <Vec4.hpp>

struct Road {
    Road();
    ~Road();

    void update(float dt, const Vec3<double>& playerPosition);
    void render() const;

private:
    static float trajectory(const unsigned int n) { return 0; }

    void pushSlab();
    void popSlab();

    float m_zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float m_slabColor[4] { 1.0f, 1.0f, 1.0f, 0.3333f };
    float m_slabColorEmission[4] { 0.0f, 1.0f, 1.0f, 1.0f };

    std::vector<Vec4<float>> m_slabs;
};
