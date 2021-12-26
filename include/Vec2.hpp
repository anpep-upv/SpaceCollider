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

template <typename T = float>
struct Vec2 {
    T x, y;

    Vec2()
        : Vec2({}, {})
    {
    }

    Vec2(const Vec2<T>& other)
        : Vec2(other.x, other.y)
    {
    }

    explicit Vec2(T all)
        : Vec2(all, all)
    {
    }

    Vec2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    T magnitude() const
    {
        return static_cast<T>(sqrt(x * x + y * y));
    }

    Vec2 normalized() const
    {
        const T m = magnitude();
        if (m == T())
            return {};

        return *this / Vec2 { m };
    }

    Vec2 operator+(const Vec2& other) const
    {
        return { x + other.x, y + other.y };
    }

    void operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
    }

    Vec2 operator-(const Vec2& other) const
    {
        return { x - other.x, y - other.y };
    }

    void operator-=(const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
    }

    Vec2 operator*(const T& value) const
    {
        return { x * value, y * value };
    }

    void operator*=(const T& value)
    {
        x *= value;
        y *= value;
    }

    Vec2 operator*(const Vec2& other) const
    {
        return { x * other.x, y * other.y };
    }

    void operator*=(const Vec2& other)
    {
        x *= other.x;
        y *= other.y;
    }

    Vec2 operator/(const T& value) const
    {
        return { x / value, y / value };
    }

    void operator/=(const T& value)
    {
        x /= value;
        y /= value;
    }

    Vec2 operator/(const Vec2& other) const
    {
        return { x / other.x, y / other.y };
    }

    void operator/=(const Vec2& other)
    {
        x /= other.x;
        y /= other.y;
    }
};
