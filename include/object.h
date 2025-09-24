#ifndef OBJECT_H
#define OBJECT_H

#include "material2d.h"
#include <raylib.h>

struct Object {
  Object(const Material2D &mat);
  ~Object();

  void Update();
  void Draw();

  Material2D mat;
};

#endif // OBJECT_H
