#include "game.h"
#include "imacros.h"
#include "object.h"
#include <cxxabi.h>
#include <memory>
#include <raylib.h>
#include <rlgl.h>
#include <string>

#if defined(GRAPHICS_API_OPENGL_33)
#if defined(_WIN32)
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif
#endif

constexpr const char *shader_path = "res/shaders/outlines.glsl";
constexpr const char *tex1_path = "res/images/sushi_clear.png";

#define COLOR_TO_VEC4(var)                                                     \
  (Vector4){var.r / 255.0f, var.g / 255.0f, var.b / 255.0f, var.a / 255.0f}

// Temp struct
struct Outline {
  Color color;
  int thickness;
};

// Temp Globals
int step = 0;
Outline outline = {{0, 0, 0, 255}, 0};

// Game
Game::Game(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height), windowTitle(title) {
  InitWindow(screenWidth, screenHeight, windowTitle.c_str());

  Vector2 pos = {960 - 256, 540 - 256};
  Vector2 size = {256 * 2, 256 * 2};

  object =
      std::make_unique<Object>(Material2D(r_manager.load<Shader>(shader_path),
                                          r_manager.load<Texture>(tex1_path)),
                               pos, size);

  object->mat.bind<Vector4>(
      "outlineColor", []() { return COLOR_TO_VEC4(outline.color); },
      SHADER_UNIFORM_VEC4);

  object->mat.bind<float>(
      "thickness", []() { return (float)outline.thickness; },
      SHADER_UNIFORM_FLOAT);

  INSPECT_INT("step", step);

  inspector.BeginGroup("Object");
  INSPECT_VEC2("pos.", object->position);
  INSPECT_VEC2("size.", object->size);

  INSPECT_COLOR("outline.", outline.color);
  INSPECT_INT("outline.thickness", outline.thickness);
  inspector.EndGroup();
}

Game::~Game() {
  r_manager.unloadAll();
  CloseWindow();
}

void Game::Run() {
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    Update(dt);
    Draw(dt);
  }
}

void Game::Update(float dt) {
  if (IsKeyPressed(KEY_M)) {
    if (IsCursorHidden())
      EnableCursor();
    else
      DisableCursor();
  }

  if (IsKeyPressed(KEY_R)) {
    rlDrawRenderBatchActive();
    glFinish();

    TraceLog(LOG_INFO, "============== RELOAD ==============");

    r_manager.force_load<Shader>(shader_path);
    r_manager.force_load<Texture>(tex1_path);

    object->mat.reload();
  }

    if (IsKeyPressed(KEY_N)) {
        TraceLog(LOG_INFO, inspector.GetName().c_str());
    }

  int val = inspector.GetName() == "step" ? 1 : step;

  if (IsKeyPressed(KEY_TAB))
    inspector.ChangeGroup(1);

  if (IsKeyPressed(KEY_RIGHT))
    inspector.Change(val);
  if (IsKeyPressed(KEY_LEFT))
    inspector.Change(-val);
  if (IsKeyPressed(KEY_UP))
    inspector.Prev();
  if (IsKeyPressed(KEY_DOWN))
    inspector.Next();
}

void Game::Draw(float dt) {
  BeginDrawing();
  ClearBackground(DARKGREEN);

  DrawFPS(8, GetScreenHeight() - 24);

  object->Draw();
  inspector.Draw();

  EndDrawing();
}
