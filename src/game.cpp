#include "game.h"
#include "iarray.h"
#include "imacros.h"
#include "object.h"
#include <cmath>
#include <cxxabi.h>
#include <functional>
#include <memory>
#include <raylib.h>
#include <rlgl.h>
#include <string>

#if defined(GRAPHICS_API_OPENGL_33)
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif
#endif

constexpr const char *shader_path = "res/shaders/outlines.glsl";
constexpr const char *tex1_path = "res/images/sushi_clear.png";

constexpr Vector4 color_to_vec4(Color color) {
  return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
          color.a / 255.0f};
}

// Temp struct
struct Outline {
  Color color;
  int thickness;
};

// Temp Globals
int step = 0;
int mantissa = 0;
int exponent = 0;
double delta = 0;

bool use_mouse = false;

Outline outline = {{0, 0, 0, 255}, 0};

iarray<std::string> ishader_paths{{
    "res/shaders/outline.glsl",
    "res/shaders/outliner.glsl",
    "res/shaders/outlines.glsl",
}};

// Game
Game::Game(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height), windowTitle(title) {
  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(screenWidth, screenHeight, windowTitle.c_str());
}

Game::~Game() {
  r_manager.unloadAll();
  CloseWindow();
}

void Game::Run() {
  Setup();
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    Update(dt);
    Draw(dt);
  }
}

void Game::Setup() {
  Vector2 size = {256 * 2, 256 * 2};
  Vector2 pos = {GetScreenWidth() / 2.f - size.x / 2.f,
                 GetScreenHeight() / 2.f - size.y / 2.f};

  object =
      std::make_unique<Object>(Material2D(r_manager.load<Shader>(shader_path),
                                          r_manager.load<Texture>(tex1_path)),
                               pos, size);

  object->mat.bind<Vector4>(
      "outlineColor", []() { return color_to_vec4(outline.color); },
      SHADER_UNIFORM_VEC4);

  object->mat.bind<float>(
      "thickness", []() { return (float)outline.thickness; },
      SHADER_UNIFORM_FLOAT);

  inspector.BeginGroup("Inspector");
  inspector.AddItem("Values", [](double d) {}, []() { return ""; });
  inspector.AddItem(
      "  Step", [&](double d) { step += std::signbit(d) ? -1 : 1; },
      [&]() { return std::to_string(step); });
  inspector.AddItem(
      "  Mantissa",
      [&](double d) {
        mantissa += (std::signbit(d) ? -1 : 1) * step;
        delta = mantissa * std::pow(10, exponent);
      },
      [&]() { return std::to_string(mantissa); });
  inspector.AddItem(
      "  Exponent",
      [&](double d) {
        exponent += (std::signbit(d) ? -1 : 1) * step;
        delta = mantissa * std::pow(10, exponent);
      },
      [&]() { return std::to_string(exponent); });
  inspector.AddItem(
      "  Use Mouse", [&](double d) { use_mouse = !use_mouse; },
      [&]() { return use_mouse ? "True" : "False"; });
  inspector.AddItem("Data", [](double d) {}, []() { return ""; });
  inspector.AddItem(
      "  Delta", [&](double delta) {}, [&]() { return std::to_string(delta); });
  inspector.EndGroup();

  inspector.BeginGroup("Object");
  inspect(inspector, "pos", object->position);
  inspect(inspector, "size", object->size);

  inspect(inspector, "outline", outline.color);
  inspect(inspector, "outline.thickness", outline.thickness);
  inspector.EndGroup();

  inspector.BeginGroup("Shader");
  inspect(inspector, "Shader path", ishader_paths);
  inspector.EndGroup();
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
    TraceLog(LOG_INFO, TextFormat("Shader Path: %s",
                                  ishader_paths.getSelected().c_str()));

    // reload shader
    object->mat.shader =
        r_manager.force_load<Shader>(ishader_paths.getSelected());
    // reload texture
    r_manager.force_load<Texture>(tex1_path);
    // reload material
    object->mat.reload();
  }

  if (IsKeyPressed(KEY_N)) {
    TraceLog(LOG_INFO, inspector.GetItemName().c_str());
  }

  float mouse = GetMouseWheelMove();
  double d = use_mouse ? static_cast<double>(mouse) * step : delta;

  if (IsKeyPressed(KEY_TAB)) {
    int dir = IsKeyDown(KEY_LEFT_SHIFT) ? -1 : 1;
    inspector.ChangeGroup(dir);
  }

  if (use_mouse && mouse != 0.) {
    TraceLog(LOG_INFO, "%lf", d);
    inspector.Change(d);
  }
  if (IsKeyPressed(KEY_RIGHT))
    inspector.Change(d);
  if (IsKeyPressed(KEY_LEFT))
    inspector.Change(-d);
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
