#ifndef OBJECT_H
#define OBJECT_H

#include "material2d.h"
#include <raylib.h>

struct Object {
  Object(const Material2D &mat, Vector2 &pos, Vector2 &size);
  ~Object();

  void Update();
  void Draw();
  void Refresh();

  Material2D mat;
  Vector2 position;
  Vector2 size;
};

#endif // OBJECT_H
