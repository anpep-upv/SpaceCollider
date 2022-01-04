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

#include <Keymap.hpp>

const std::map<unsigned char, Keymap::InputCommand> Keymap::m_keymap {
#ifndef KEYMAP_CLASSIC
    // Modern style
    { 'w', InputCommand::Thrust },
    { 'a', InputCommand::TurnLeft },
    { 's', InputCommand::Brake },
    { 'd', InputCommand::TurnRight },

    { '1', InputCommand::ToggleWireframeView },
    { '2', InputCommand::ToggleLightingMode },
    { '3', InputCommand::ToggleFog },
    { '4', InputCommand::ToggleHUD },

    { '5', InputCommand::ToggleBirdView },
    { '6', InputCommand::ToggleSkybox },
    { '7', InputCommand::ToggleConsole },
    { '8', InputCommand::ToggleMotionBlur },
    { '9', InputCommand::ToggleMSAA },

    { 'm', InputCommand::ToggleSoundtrack },
#else
    // Required style
    { KEY_TRANSLATION_UP, InputCommand::Thrust },
    { KEY_TRANSLATION_LEFT, InputCommand::TurnLeft },
    { KEY_TRANSLATION_DOWN, InputCommand::Brake },
    { KEY_TRANSLATION_RIGHT, InputCommand::TurnRight }

    { 's', InputCommand::ToggleWireframeView },
    { 'l', InputCommand::ToggleLightingMode },
    { 'n', InputCommand::ToggleFog },
    { 'c', InputCommand::ToggleHUD },

    { 'b', InputCommand::ToggleBirdView },
    { 'k', InputCommand::ToggleSkybox },
    { 'o', InputCommand::ToggleConsole },
    { 'm', InputCommand::ToggleMotionBlur },
    { 'a', InputCommand::ToggleMSAA },

    { 'u', InputCommand::ToggleSoundtrack }
#endif // KEYMAP_CLASSIC
};