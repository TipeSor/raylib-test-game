#include "game.h"
#include "object.h"
#include <cstddef>
#include <memory>
#include <raylib.h>
#include <rlgl.h>
#include <utility>

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

#define INSPECT(var) inspector.Add(#var, &var);

// Test inspector
struct Inspector {
public:
  void Add(const std::string &name, int *value) {
    if (value)
      data.push_back({name, value});
  }

  void Next() {
    if (!data.empty())
      selected = (selected + 1) % data.size();
  }

  void Prev() {
    if (!data.empty())
      selected = (selected == 0) ? data.size() - 1 : selected - 1;
  }

  void Change(int value) {
    if (!data.empty() && data[selected].second) {
      *(data[selected].second) += value;
    }
  }

  void Draw() {
    if (!data.empty()) {
      for (size_t index = 0; index < data.size(); index++) {
        auto &[name, ptr] = data[index];
        if (ptr)
          DrawText(TextFormat(" %c %s: %i", (index == selected ? '>' : ' '),
                              name.c_str(), *ptr),
                   0, 32*index, 32, WHITE);
      }
    }
  }

  std::string GetName() {
    if (!data.empty() && data[selected].second) {
      return data[selected].first;
    }
    return "";
  }

private:
  std::vector<std::pair<std::string, int *>> data;
  size_t selected = 0;
};

struct Outline {
  int r;
  int g;
  int b;
  int a;
  int thickness;
};

// Temp Globals
int step = 0;
Outline outline = {0, 0, 0, 255, 0};

Inspector inspector;

// Game
Game::Game(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height), windowTitle(title) {
  InitWindow(screenWidth, screenHeight, windowTitle.c_str());

  Vector2 pos = {960-256, 540-256};
  Vector2 size = {256 * 2, 256 * 2};

  object =
      std::make_unique<Object>(Material2D(r_manager.load<Shader>(shader_path),
                                          r_manager.load<Texture>(tex1_path)),
                               pos, size);

  object->mat.bind<Vector4>(
      "outlineColor",
      []() {
        return (Vector4){outline.r / 255.0f, outline.g / 255.0f,
                         outline.b / 255.0f, outline.a / 255.0f};
      },
      SHADER_UNIFORM_VEC4);

  object->mat.bind<float>(
      "thickness", []() { return (float)outline.thickness; },
      SHADER_UNIFORM_FLOAT);

  INSPECT(step);
  INSPECT(outline.r);
  INSPECT(outline.g);
  INSPECT(outline.b);
  INSPECT(outline.a);
  INSPECT(outline.thickness);
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

  int val = inspector.GetName() == "step" ? 1 : step;

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

  DrawFPS(0, GetScreenHeight() - 16);
  object->Draw();

  inspector.Draw();

  EndDrawing();
}
