#include "object.h"
#include "material2d.h"
#include <raylib.h>

Object::Object(const Material2D &mat) : mat(mat) {}

Object::~Object() {}

void Object::Update() {}

void Object::Draw() {
  float time = GetTime();
  Vector2 mouse = GetMousePosition();
  mouse.y = GetScreenHeight() - mouse.y;

  mat.SetShaderValue("time", &time, SHADER_UNIFORM_FLOAT);
  mat.SetShaderValue("mouse", &mouse, SHADER_UNIFORM_VEC2);

  DrawMaterial2DPro(mat,
                    {0, 0, (float)mat.baseTexture.width,
                     (float)mat.baseTexture.height},
                    {0, 0, 512, 512}, {0, 0}, 0.0f, WHITE);
}
