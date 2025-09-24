#include "game.h"
#include "object.h"
#include <optional>
#include <raylib.h>

Texture tex1;
Texture tex2;
Shader shader;
const char *shader_path = "res/shaders/outline.glsl";

std::optional<Object> object;

Game::Game(int width, int height, const char *title)
    : screenWidth(width), screenHeight(height), windowTitle(title) {
  InitWindow(screenWidth, screenHeight, windowTitle);

  tex1 = LoadTexture("res/images/mijo_clear.png");
  tex2 = LoadTexture("res/images/sushi_clear.png");

  shader = LoadShader(NULL, shader_path);

  object = Object(Material2D(shader, {tex1, tex2}));
}

Game::~Game() {
  UnloadTexture(tex1);
  UnloadShader(shader);
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
    Shader newShader = LoadShader(NULL, shader_path);
    if (newShader.id != 0) {
      UnloadShader(shader);
      shader = newShader;
      object = Object(Material2D(shader, {tex1}));
    } else {
      TraceLog(LOG_ERROR, "Shader reload failed.");
    }
  }
}

void Game::Draw(float dt) {
  BeginDrawing();
  ClearBackground(BLACK);

  if (object.has_value())
    object.value().Draw();

  EndDrawing();
}
