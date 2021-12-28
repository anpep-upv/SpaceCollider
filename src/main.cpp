// SpaceCollider -- SGI Lab Task 9
// Copyright (c) 2021-2022 Ãngel PÃ©rez <aperpor@upv.edu.es>
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

#include <GL/freeglut.h>
#include <HUD.hpp>
#include <PlatformQuirks.hpp>
#include <Player.hpp>
#include <Road.hpp>
#include <Util.hpp>

static int g_viewportWidth = 1024;
static int g_viewportHeight = 768;

static constexpr int k_maxFps = 60;
static int g_fps = 0;
static int g_frameCounter = 0;
static int g_lastFrameTime = 0, g_lastFpsTime = 0;

static struct {
    bool initialized;
    std::unique_ptr<Player> player;
    std::unique_ptr<Model> mothership;
    std::unique_ptr<Road> road;

    // Mothership model advance
    float mothershipAdvance = 0.0f;
} g_scene;

static void updateFpsCounter()
{
    const auto currentTime = glutGet(GLUT_ELAPSED_TIME);
    const auto dt = currentTime - g_lastFpsTime;

    g_frameCounter++;
    if (dt >= 1000) {
        // One second has elapsed
        g_fps = g_frameCounter;
        g_frameCounter = 0;
        g_lastFpsTime = currentTime;
    }
}

static void onTimer(const int value)
{
    const auto currentTime = glutGet(GLUT_ELAPSED_TIME);
    const float dt = g_lastFrameTime == 0 ? 0.0f : (static_cast<float>(currentTime) - static_cast<float>(g_lastFrameTime)) / 1000.0f;

    // Update player properties
    g_scene.player->update(dt);

    // Update mothership position
    g_scene.mothershipAdvance += 25 * dt;

    // Update road properties
    g_scene.road->update(dt, *g_scene.player);

    // Display FPS counter
    Util::consolePrint("%d FPS", g_fps);

    // Reschedule timer
    g_lastFrameTime = currentTime;
    glutTimerFunc(1000 / k_maxFps, onTimer, value);
    glutPostRedisplay();
}

static void onDisplay()
{
    if (!g_scene.initialized)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Multisample antialiasing
    if (g_scene.player->isMsaaEnabled())
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

    // Camera control
    g_scene.player->updateCamera();

    // Render scene objects
    glPushMatrix();
    {
        // Mothership matrix
        glTranslatef(100.0f, 0.0f, 100.0f + g_scene.mothershipAdvance);
        g_scene.mothership->render(0.01f);
        glPopMatrix();
    }

    g_scene.player->render();

    glPushMatrix();
    {
        // Road matrix
        // The road object must be rendered the last in order to achieve alpha blending
        glTranslatef(0.0f, -2.5f, 0.0f);
        g_scene.road->render();
        glPopMatrix();
    }

    // Console control
    if (g_scene.player->isConsoleVisible())
        Util::renderOverlayString(Util::s_consoleBuffer, Util::k_consoleFontSize + 1, Util::k_consoleFontSize * Util::s_consoleLines - 1);

    Util::consoleClear();
    updateFpsCounter();

    // Motion blur
    if (g_scene.player->isMotionBlurEnabled()) {
        glAccum(GL_MULT, 0.5f);
        glAccum(GL_ACCUM, 1.0f - 0.5f);
        glAccum(GL_RETURN, 1.0f);
    }

    glutSwapBuffers();
}

static void onReshape(const int width, const int height)
{
    g_viewportWidth = width;
    g_viewportHeight = height;
    Player::updateViewport(width, height);
}

static void onKeyboard(const int up, const unsigned char key, const int x, const int y)
{
    g_scene.player->handleKeyboardEvent(up, key, x, y);
}

static void onSpecialKeyboard(const int up, int specialKey, const int x, const int y)
{
    unsigned char translatedKey;

    switch (specialKey) {
    case GLUT_KEY_UP:
        translatedKey = 'w';
        break;
    case GLUT_KEY_LEFT:
        translatedKey = 'a';
        break;
    case GLUT_KEY_DOWN:
        translatedKey = 's';
        break;
    case GLUT_KEY_RIGHT:
        translatedKey = 'd';
        break;
    default:
        translatedKey = static_cast<unsigned char>(specialKey & 0xFF);
        break;
    }

    onKeyboard(up, translatedKey, x, y);
}

static void onKeyboardUp(const unsigned char key, const int x, const int y)
{
    onKeyboard(1, key, x, y);
}

static void onKeyboardDown(const unsigned char key, const int x, const int y)
{
    onKeyboard(0, key, x, y);
}

static void onSpecialKeyboardUp(const int key, const int x, const int y)
{
    onSpecialKeyboard(1, key, x, y);
}

static void onSpecialKeyboardDown(const int key, const int x, const int y)
{
    onSpecialKeyboard(0, key, x, y);
}

static void initScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    g_scene.player = std::make_unique<Player>();
    g_scene.mothership = std::make_unique<Model>("data/EMPFLT/EMPFLT.obj");
    g_scene.road = std::make_unique<Road>();
    g_scene.initialized = true;
}

static void destroyScene()
{
    g_scene.mothershipAdvance = 0.0f;
    g_scene.initialized = false;
    g_scene.road.reset();
    g_scene.mothership.reset();
    g_scene.player.reset();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA | GLUT_ACCUM | GLUT_MULTISAMPLE);
    glutInitWindowSize(g_viewportWidth, g_viewportHeight);
    glutCreateWindow("SpaceCollider");

    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);

    // Keyboard control
    glutKeyboardFunc(onKeyboardDown);
    glutKeyboardUpFunc(onKeyboardUp);
    glutSpecialFunc(onSpecialKeyboardDown);
    glutSpecialFunc(onSpecialKeyboardUp);

    glutIgnoreKeyRepeat(true);
    glutTimerFunc(1000 / k_maxFps, onTimer, 0);
    initScene();
    glutMainLoop();
    destroyScene();

    return 0;
}
