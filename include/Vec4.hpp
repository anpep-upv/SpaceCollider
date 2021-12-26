// GraphicsKit -- Utility library for OpenGL applications
// Copyright (c) 2021 Ángel Pérez <aperpor@upv.edu.es>
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

#include <Util.hpp>

template <typename T = float>
struct Vec4 {
    T x, y, z, w;

    Vec4()
        : Vec4({}, {}, {}, {})
    {
    }

    Vec4(const Vec4<T>& other)
        : Vec4(other.x, other.y, other.z, other.w)
    {
    }

    explicit Vec4(T all)
        : Vec4(all, all, all, all)
    {
    }

    Vec4(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    T magnitude() const
    {
        return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
    }

    Vec4 normalized() const
    {
        const T m = magnitude();
        if (m == T())
            return {};

        return *this / Vec4 { m };
    }
    

#pragma region Operators
    Vec4 operator+(const Vec4& other) const
    {
        return { x + other.x, y + other.y, z + other.z, w + other.w };
    }

    void operator+=(const Vec4& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    Vec4 operator-(const Vec4& other) const
    {
        return { x - other.x, y - other.y, z - other.z, w - other.w };
    }

    void operator-=(const Vec4& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    Vec4 operator*(const T& value) const
    {
        return { x * value, y * value, z * value, w * value };
    }

    void operator*=(const T& value)
    {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
    }

    Vec4 operator*(const Vec4& other) const
    {
        return { x * other.x, y * other.y, z * other.z, w * other.w };
    }

    void operator*=(const Vec4& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
    }

    Vec4 operator/(const T& value) const
    {
        return { x / value, y / value, z / value, w / value };
    }

    void operator/=(const T& value)
    {
        x /= value;
        y /= value;
        z /= value;
        w /= value;
    }

    Vec4 operator/(const Vec4& other) const
    {
        return { x / other.x, y / other.y, z / other.z, w / other.w };
    }

    void operator/=(const Vec4& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
    }
#pragma endregion
};
