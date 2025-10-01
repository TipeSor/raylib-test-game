#include "game.h"
#include "object.h"
#include <memory>
#include <raylib.h>

const char *shader_path = "res/shaders/outliner.glsl";
const char *tex1_path = "res/images/mijo_clear.png";
const char *tex2_path = "res/images/sushi_clear.png";

Game::Game(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height), windowTitle(title) {
  InitWindow(screenWidth, screenHeight, windowTitle);

  Vector2 pos = {0, 0};
  Vector2 size = {256 * 2, 256 * 2};

  object =
      std::make_unique<Object>(Material2D(r_manager.load<Shader>(shader_path),
                                          r_manager.load<Texture>(tex1_path)),
                               pos, size);

  object->mat.bind("texture1", r_manager.load<Texture>(tex2_path),
                   SHADER_UNIFORM_SAMPLER2D);
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
    TraceLog(LOG_INFO, "============== RELOAD ==============");
    r_manager.force_load<Shader>(shader_path);
    Texture &tex1 = r_manager.force_load<Texture>(tex1_path);
    Texture &tex2 = r_manager.force_load<Texture>(tex2_path);

    object->mat.texture = &tex1;
    object->mat.unbind("texture1");
    object->mat.bind("texture1", tex2, SHADER_UNIFORM_SAMPLER2D);

    object->mat.reload();
  }
}

void Game::Draw(float dt) {
  BeginDrawing();
  ClearBackground(BLACK);

  object->Draw();

  EndDrawing();
}
