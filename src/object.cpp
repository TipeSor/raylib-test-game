#include "object.h"
#include "material2d.h"
#include <raylib.h>

Object::Object(Material2D &&mat, Vector2 &pos, Vector2 &size)
    : mat(std::move(mat)), position(pos), size(size) {

  this->mat.bind<Vector2>(
      "mouse",
      []() {
        Vector2 mouse = GetMousePosition();
        mouse.y = GetScreenHeight() - mouse.y;
        return mouse;
      },
      SHADER_UNIFORM_VEC2);

  this->mat.bind<float>(
      "time", []() { return GetTime(); }, SHADER_UNIFORM_FLOAT);
}

Object::~Object() {}

void Object::Update() {}

void Object::Draw() {
  float width = (float)mat.texture.width;
  float height = (float)mat.texture.height;

  Rectangle source = {0, 0, width, height};
  Rectangle dest = {position.x, position.y, size.x, size.y};

  mat.DrawPro(source, dest, {0, 0}, 0.0f, WHITE);
}
