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

#include "Util.hpp"

#include <Skybox.hpp>

Skybox::Skybox()
    : m_tex_back(Texture{ "data/skybox_back.png" })
      , m_tex_front(Texture{ "data/skybox_front.png" })
      , m_tex_left(Texture{ "data/skybox_left.png" })
      , m_tex_right(Texture{ "data/skybox_right.png" })
      , m_tex_bottom(Texture{ "data/skybox_bottom.png" })
      , m_tex_top(Texture{ "data/skybox_top.png" }) {}

void Skybox::render(const Vec3<double>& center, const bool isBirdView, const bool isVisible) const
{
    if (!isVisible)
        return;

    glPushMatrix();
    glTranslatef(0.0f, -1.5f, 0.0f);
    glScalef(20, 20, 20);
    {
        m_tex_front.bind();
        glPushMatrix();
        {
            glTranslatef(-0.5f, 0.0f, 0.5f);

            glBegin(GL_QUADS);
            glVertex3f(0, 0, 0.0f);
            glVertex3f(0, 1, 0.0f);
            glVertex3f(1, 1, 0.0f);
            glVertex3f(1, 0, 0.0f);
            glEnd();

            glPopMatrix();
        }

        m_tex_back.bind();
        glPushMatrix();
        {
            glTranslatef(0.5f, 0.0f, -0.5f);
            glRotatef(180, 0.0f, 1.0f, 0.0f);

            glBegin(GL_QUADS);
            glVertex3f(0, 0, 0.0f);
            glVertex3f(0, 1, 0.0f);
            glVertex3f(1, 1, 0.0f);
            glVertex3f(1, 0, 0.0f);
            glEnd();

            glPopMatrix();
        }

        m_tex_right.bind();
        glPushMatrix();
        {
            glTranslatef(0.5f, 0.0f, 0.5f);
            glRotatef(90, 0.0, 1.0f, 0.0f);

            glBegin(GL_QUADS);
            glVertex3f(0, 0, 0.0f);
            glVertex3f(0, 1, 0.0f);
            glVertex3f(1, 1, 0.0f);
            glVertex3f(1, 0, 0.0f);
            glEnd();

            glPopMatrix();
        }

        m_tex_left.bind();
        glPushMatrix();
        {
            glTranslatef(-0.5f, 0.0, -0.5f);
            glRotatef(-90, 0.0, 1.0f, 0.0f);

            glBegin(GL_QUADS);
            glVertex3f(0, 0, 0.0f);
            glVertex3f(0, 1, 0.0f);
            glVertex3f(1, 1, 0.0f);
            glVertex3f(1, 0, 0.0f);
            glEnd();

            glPopMatrix();
        }

        m_tex_bottom.bind();
        glPushMatrix();
        {
            glTranslatef(-0.5f, 0.0f, -0.5f);
            glRotatef(90, 1.0f, 0.0f, 0.0f);

            glBegin(GL_QUADS);
            glVertex3f(0, 0, 0.0f);
            glVertex3f(0, 1, 0.0f);
            glVertex3f(1, 1, 0.0f);
            glVertex3f(1, 0, 0.0f);
            glEnd();

            glPopMatrix();
        }

        if (!isBirdView) {
            // Don't render top plane so that it does not interfere with bird view
            m_tex_top.bind();
            glPushMatrix();
            {
                glTranslatef(-0.5f, 1.0f, 0.5f);
                glRotatef(-90, 1.0f, 0.0f, 0.0f);

                glBegin(GL_QUADS);
                glVertex3f(0, 0, 0.0f);
                glVertex3f(0, 1, 0.0f);
                glVertex3f(1, 1, 0.0f);
                glVertex3f(1, 0, 0.0f);
                glEnd();

                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}
