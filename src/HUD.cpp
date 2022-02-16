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

#include <PlatformQuirks.hpp>
#include <PlatformGraphics.h>
#include <HUD.hpp>

void HUD::renderBar(unsigned int index, const std::string& label, float value)
{
    glPushMatrix();
    {
        glTranslatef(2.0f, 0.5f * index - 0.75f, 0.0f);

        // Render bar label
        glPushMatrix();
        {
            glScalef(-0.00075f, 0.00075f, 0.001f);
            glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
            for (const auto character : label)
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, character);
            glPopMatrix();
        }

        glPushMatrix();
        {
            glTranslatef(-1.0f, -0.25f, 0.0f);
            glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
            glRectf(0.0f, 0.20f, 1.0f, 0.18f);
            glPopMatrix();
        }

        glPushMatrix();
        {
            glTranslatef(1.0f - value - 1.0f, -0.25f, 0.0f);
            if (value < 0.25)
                glColor4f(1.0f, 0.25f, 0.25f, 0.75f);
            else
                glColor4f(0.5f, 1.0f, 1.0f, 0.75f);
            glRectf(0.0f, 0.0f, 1.0f * value, 0.15f);
            glPopMatrix();
        }

        glPopMatrix();
    }
}

void HUD::render() const
{
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderBar(0, "FUEL", m_fuel);
    renderBar(1, "SPEED", m_speed);

    glDepthMask(GL_TRUE);
    glPopAttrib();
}