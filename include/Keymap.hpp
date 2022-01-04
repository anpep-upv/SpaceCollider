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

#include <map>

struct Keymap {
    enum class InputCommand {
        // Player control
        Thrust,
        Brake,
        TurnLeft,
        TurnRight,

        // Option toggles
        ToggleWireframeView,
        ToggleLightingMode,
        ToggleFog,
        ToggleHUD,

        // Debug toggles
        ToggleBirdView,
        ToggleSkybox,
        ToggleConsole,
        ToggleMotionBlur,
        ToggleMSAA,

        // Soundtrack control
        ToggleSoundtrack
    };

    static const std::map<unsigned char, InputCommand>& the() { return m_keymap; }

private:
    static const std::map<unsigned char, InputCommand> m_keymap;
};