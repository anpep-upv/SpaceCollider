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

#include <Model.hpp>

static constexpr int k_maxFps = 60;
static Model* g_model = nullptr;
static float g_angle = 0;

static void init()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

static void onDisplay()
{
    if (!g_model)
        g_model = new Model{ "data/PLAHB1/PLAHB1.obj" };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    
    constexpr float lightAmbient[] = { 0.85f, 0.85f, 0.85f, 0.85f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);

    glRotatef(g_angle, 0.0f, 1.0f, 0.0f);
    g_model->render(0.001f);

    glutSwapBuffers();
}

static void onReshape(const int width, const int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, static_cast<double>(width) / static_cast<double>(height), 0.1, 100.0);
}

static void onTimer(int)
{
    g_angle += 1.0f;
    glutPostRedisplay();
    glutTimerFunc(1000 / k_maxFps, onTimer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("ModelTest");
    init();
    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);
    glutTimerFunc(1000 / k_maxFps, onTimer, 0);
    glutMainLoop();

    return 0;
}
