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

#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <PlatformQuirks.hpp>
#include <PlatformGraphics.h>
#include <Util.hpp>

char Util::s_consoleBuffer[512] {}; // Console text buffer
int Util::s_consoleLines { 0 }; // Console line count

void Util::consolePrint(const char* format, ...)
{
    const auto consoleLength = strlen(s_consoleBuffer);
    va_list argList;
    va_start(argList, format);
    vsnprintf_s(s_consoleBuffer + consoleLength, sizeof s_consoleBuffer - consoleLength, sizeof s_consoleBuffer - consoleLength - 1, format, argList);
    strncat_s(s_consoleBuffer, sizeof s_consoleBuffer - 1, "\n", 2);
    s_consoleLines++;
    va_end(argList);
}

void Util::consoleClear()
{
    memset(s_consoleBuffer, 0, sizeof s_consoleBuffer);
    s_consoleLines = 0;
}

void Util::renderOverlayString(const char* string, const int x, int y)
{
    int viewport[4] {};
    glGetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint*>(&viewport));

    glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        glLoadIdentity();
        gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        {
            glLoadIdentity();
            glRasterPos2i(x, y);
            bool colorChanged = false;
            while (*string) {
                if (*string == '\x01') {
                    // Reset
                    glColor3f(1.0f, 1.0f, 1.0f);
                    colorChanged = true;
                    string++;
                } else if (*string == '\x02') {
                    // Red
                    glColor3f(1.0f, 0.0f, 0.0f);
                    colorChanged = true;
                    string++;
                } else if (*string == '\x03') {
                    // Green
                    glColor3f(0.0f, 1.0f, 0.0f);
                    colorChanged = true;
                    string++;
                } else if (*string == '\x04') {
                    // Cyan
                    glColor3f(0.0f, 1.0f, 1.0f);
                    colorChanged = true;
                    string++;
                } else if (*string == '\x05') {
                    // Gray
                    glColor3f(0.5f, 0.5f, 0.5f);
                    colorChanged = true;
                    string++;
                }

                if (colorChanged) {
                    int currentPosition[4] = {};
                    glGetIntegerv(GL_CURRENT_RASTER_POSITION, currentPosition);
                    glRasterPos2iv(currentPosition);
                    colorChanged = false;
                    continue;
                }

                if (*string == '\n') {
                    string++;
                    y -= k_consoleFontSize;
                    glRasterPos2i(x, y);
                } else {
                    glutBitmapCharacter(k_consoleFont, *string++);
                }
            }
        }
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    }

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

std::vector<std::string> Util::splitString(const std::string& string, const char delimiter)
{
    std::vector<std::string> segmentVector;
    std::stringstream stringStream { string };
    std::string segment;

    while (std::getline(stringStream, segment, delimiter))
        segmentVector.push_back(segment);

    return segmentVector;
}