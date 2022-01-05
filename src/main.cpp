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

#include <algorithm>

#include <Keymap.hpp>
#include <MothershipModel.hpp>
#include <PlatformQuirks.hpp>
#include <GL/freeglut.h>
#include <Player.hpp>
#include <SoundtrackManager.hpp>
#include <Tunnel.hpp>
#include <Util.hpp>

// Viewport properties
static int g_viewportWidth = 1024;
static int g_viewportHeight = 768;

// Frame rate control
static constexpr int k_maxFps = 60;
static int g_fps = 0, g_worstFps = 0, g_bestFps = 0;
static int g_frameCounter = 0;
static int g_lastFrameTime = 0, g_lastFpsTime = 0;

// Scene
static struct {
    bool isInitialized = false;

    std::unique_ptr<Player> player;
    std::unique_ptr<Model> skybox;
    std::unique_ptr<MothershipModel> mothership;
    std::unique_ptr<Tunnel> tunnel;
} g_scene;

// Debug toggles
static bool g_isFogEnabled = false;
static bool g_isSkyboxVisible = true;
static bool g_isMSAAEnabled = true;
static bool g_isConsoleVisible = true;
static bool g_isMotionBlurEnabled = false;
static bool g_isNightModeEnabled = false;
static bool g_isWireframeViewEnabled = false;

static void initSkybox()
{
    g_scene.skybox = std::make_unique<Model>("data/skybox/skybox.obj", Vec3(), Vec3(1500.0f));

    if (g_isNightModeEnabled)
        g_scene.skybox->getMaterialCollection()->getMaterial("Material").texture = std::make_unique<Texture>("data/skybox/skybox0.png");
    else
        g_scene.skybox->getMaterialCollection()->getMaterial("Material").texture = std::make_unique<Texture>("data/skybox/skybox4.png");
}

static void initScene()
{
    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    g_scene.player = std::make_unique<Player>();
    initSkybox();
    g_scene.mothership = std::make_unique<MothershipModel>();
    g_scene.tunnel = std::make_unique<Tunnel>();

    for (const auto& entry : std::filesystem::directory_iterator("data/soundtrack")) {
        if (!entry.is_regular_file() || entry.path().extension().string() != ".wav")
            continue;
        SoundtrackManager::the().enqueueTrack(entry.path().string());
    }

    // SoundtrackManager::the().playNextTrack();
    g_scene.isInitialized = true;
}

static void destroyScene()
{
    SoundtrackManager::the().stopTrack();
    g_scene.isInitialized = false;
    g_scene.tunnel.reset();
    g_scene.mothership.reset();
    g_scene.player.reset();
}

static void updateFpsCounter()
{
    const auto currentTime = glutGet(GLUT_ELAPSED_TIME);
    const auto dt = currentTime - g_lastFpsTime;

    g_frameCounter++;
    if (dt >= 1000) {
        // One second has elapsed
        g_fps = g_frameCounter;
        g_bestFps = std::max(g_fps, g_bestFps);
        g_worstFps = std::min(g_fps, g_worstFps);
        g_frameCounter = 0;
        g_lastFpsTime = currentTime;
    }
}

static void onTimer(const int value)
{
    const auto currentTime = glutGet(GLUT_ELAPSED_TIME);
    const float dt = g_lastFrameTime == 0 ? 0.0f : (static_cast<float>(currentTime) - static_cast<float>(g_lastFrameTime)) / 1000.0f;

    g_scene.player->update(dt);
    if (g_scene.player->isEngineOn())
        g_scene.skybox->setPosition(g_scene.player->getPosition());
    g_scene.mothership->update(dt);
    g_scene.tunnel->update(dt, *g_scene.player);

    // Display FPS counter
    Util::consolePrint("FPS");
    Util::consolePrint("  \5CUR\1  %d f/s", g_fps);
    Util::consolePrint("  \5MIN\1  %d f/s    \5MAX\1  %d f/s", g_worstFps, g_bestFps);

    // Display state
    Util::consolePrint("DBUG");
    Util::consolePrint("  \5VIW\1  %s\1", !g_isWireframeViewEnabled ? "\3TEX" : "\2WFR");
    Util::consolePrint("  \5FOG\1  %s\1", g_isFogEnabled ? "\3YES" : "\2NO");
    Util::consolePrint("  \5ACUM\1 %s\1", g_isMotionBlurEnabled ? "\3YES" : "\x02NO");
    Util::consolePrint("  \5MSAA\1 %s\1", g_isMSAAEnabled ? "\3YES" : "\2NO");

    // Reschedule timer
    g_lastFrameTime = currentTime;
    glutTimerFunc(1000 / k_maxFps, onTimer, value);
    glutPostRedisplay();
}

static void onDisplay()
{
    if (!g_scene.isInitialized)
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (g_isWireframeViewEnabled) {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (!g_isNightModeEnabled) {
        constexpr float ambientColor[]{ 0.722f, 0.745f, 0.286f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
        glFogfv(GL_FOG_COLOR, ambientColor);
    }
    else {
        constexpr float ambientColor[]{ 0.2f, 0.2f, 0.2f, 1.0f };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
        glFogfv(GL_FOG_COLOR, ambientColor);
    }

    // Multisample antialiasing
    if (g_isMSAAEnabled)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

    // Fog
    if (g_isFogEnabled) {
        glEnable(GL_FOG);
        glFogi(GL_FOG_MODE, GL_EXP2);
        
        glFogf(GL_FOG_DENSITY, 0.00075f);
    } else {
        glDisable(GL_FOG);
    }

    // Camera control
    g_scene.player->updateCamera();

    // Render scene objects
    if (g_isSkyboxVisible)
        g_scene.skybox->render();

    g_scene.mothership->render();
    g_scene.tunnel->render();
    g_scene.player->render();

    if (g_isConsoleVisible)
        Util::renderOverlayString(Util::s_consoleBuffer, Util::k_consoleFontSize + 1, Util::k_consoleFontSize * Util::s_consoleLines - 1);

    if (SoundtrackManager::the().isTrackPlaying())
        Util::renderOverlayString(("\5Now Playing:\1 " + SoundtrackManager::the().getTrackName()).c_str(), 16, g_viewportHeight - Util::k_consoleFontSize - 16);

    Util::consoleClear();
    updateFpsCounter();

    if (g_isMotionBlurEnabled) {
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
    if (!Keymap::the().contains(key))
        return;

    switch (Keymap::the().at(key)) {
    case Keymap::InputCommand::ToggleWireframeView:
        if (up)
            g_isWireframeViewEnabled = !g_isWireframeViewEnabled;
        break;
    case Keymap::InputCommand::ToggleLightingMode:
        if (up) {
            g_isNightModeEnabled = !g_isNightModeEnabled;
            initSkybox();
        }
        break;
    case Keymap::InputCommand::ToggleFog:
        if (up)
            g_isFogEnabled = !g_isFogEnabled;
        break;
    case Keymap::InputCommand::ToggleSkybox:
        if (up)
            g_isSkyboxVisible = !g_isSkyboxVisible;
        break;
    case Keymap::InputCommand::ToggleConsole:
        if (up)
            g_isConsoleVisible = !g_isConsoleVisible;
        break;
    case Keymap::InputCommand::ToggleMotionBlur:
        if (up)
            g_isMotionBlurEnabled = !g_isMotionBlurEnabled;
        break;
    case Keymap::InputCommand::ToggleMSAA:
        if (up)
            g_isMSAAEnabled = !g_isMSAAEnabled;
        break;
    case Keymap::InputCommand::ToggleSoundtrack:
        if (up) {
            if (SoundtrackManager::the().isTrackPlaying())
                SoundtrackManager::the().stopTrack();
            else
                SoundtrackManager::the().playNextTrack();
        }
        break;
    default:
        g_scene.player->handleKeyboardEvent(up, key, x, y);
        break;
    }
}

static void onSpecialKeyboard(const int up, int specialKey, const int x, const int y)
{
    unsigned char translatedKey;

    switch (specialKey) {
    case GLUT_KEY_UP:
        translatedKey = KEY_TRANSLATION_UP;
        break;
    case GLUT_KEY_LEFT:
        translatedKey = KEY_TRANSLATION_LEFT;
        break;
    case GLUT_KEY_DOWN:
        translatedKey = KEY_TRANSLATION_DOWN;
        break;
    case GLUT_KEY_RIGHT:
        translatedKey = KEY_TRANSLATION_RIGHT;
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
