#include "game.h"
#include "object.h"
#include <memory>
#include <raylib.h>

const char *shader_path = "res/shaders/outliner.glsl";
const char *tex1_path = "res/images/mijo_clear.png";
const char *tex2_path = "res/images/mijo_clear.png";

std::unique_ptr<Object> object;

Game::Game(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height), windowTitle(title) {
  InitWindow(screenWidth, screenHeight, windowTitle);

  Material2D mat = Material2D(
      r_manager.load<Shader>(shader_path),
      {r_manager.load<Texture>(tex1_path), r_manager.load<Texture>(tex2_path)});
  Vector2 pos = {0, 0};
  Vector2 size = {256 * 2, 256 * 2};

  object = std::make_unique<Object>(Object(mat, pos, size));
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

  // Will fail if shader has errors.... oh well
  if (IsKeyPressed(KEY_R)) {
    r_manager.unload(object->mat.shader);
    r_manager.unload(object->mat.baseTexture);
    for (auto &it : object->mat.textureBindings)
      r_manager.unload(it.second);

    object->mat = Material2D(r_manager.load<Shader>(shader_path),
                             {r_manager.load<Texture>(tex1_path),
                              r_manager.load<Texture>(tex2_path)});
  }
}

void Game::Draw(float dt) {
  BeginDrawing();
  ClearBackground(BLACK);

    object->Draw();

  EndDrawing();
}
