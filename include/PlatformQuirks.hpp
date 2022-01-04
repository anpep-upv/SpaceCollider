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

// We could easily get rid of these by defining _CRT_SECURE_NO_WARNINGS but there's no harm in having these defined in macros
#if !defined(_MSC_VER) && !defined(vsnprintf_s)
#define vsnprintf_s(buf, bufsz, count, format, args) vsnprintf(buf, count, format, args)
#endif

#if !defined(_MSC_VER) && !defined(strncat_s)
#define strncat_s(dest, destsz, src, count) strncat(dest, src, count)
#endif

// GL_MULTISAMPLE is not defined in the supplied codebase GL.h but is actually soported by most modern drivers
// This is not needed to enable MSAA but it is required to enable/disable it on the fly
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

#define KEY_TRANSLATION_UP 0x80
#define KEY_TRANSLATION_LEFT 0x81
#define KEY_TRANSLATION_DOWN 0x82
#define KEY_TRANSLATION_RIGHT 0x83