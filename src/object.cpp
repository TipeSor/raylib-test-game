#include "object.h"
#include "material2d.h"
#include <raylib.h>

Object::Object(const Material2D &mat, Vector2 &pos, Vector2 &size)
    : mat(mat), position(pos), size(size) {}

Object::~Object() {}

void Object::Update() {}

void Object::Draw() {
  float time = GetTime();
  Vector2 mouse = GetMousePosition();
  mouse.y = GetScreenHeight() - mouse.y;

  mat.SetShaderValue("time", &time, SHADER_UNIFORM_FLOAT);
  mat.SetShaderValue("mouse", &mouse, SHADER_UNIFORM_VEC2);

  float width = (float)mat.baseTexture.width;
  float height = (float)mat.baseTexture.height;

  Rectangle source = {0, 0, width, height};

  Rectangle dest = {position.x, position.y, size.x, size.y};

  DrawMaterial2DPro(mat, source, dest, {0, 0}, 0.0f, WHITE);
}
