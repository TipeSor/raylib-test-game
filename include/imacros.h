#ifndef IMACROS_H
#define IMACROS_H

#define INSPECT_INT(name, var)                                                 \
  inspector.AddItem(                                                           \
      name, &var, [&](float delta) { var += static_cast<int>(delta); },        \
      [&]() { return std::to_string(var); });

#define INSPECT_FLOAT(name, var)                                               \
  inspector.AddItem(                                                           \
      name, &var, [&](float delta) { var += delta; },                          \
      [&]() { return std::to_string(var); });

#define INSPECT_UCHAR(name, var)                                               \
  inspector.AddItem(                                                           \
      name, &var,                                                              \
      [&](float delta) {                                                       \
        var = std::clamp(var + static_cast<int>(delta), 0, 255);               \
      },                                                                       \
      [&]() { return std::to_string(var); });

#define INSPECT_VEC2(name, var)                                                \
  INSPECT_FLOAT(name "x", var.x);                                             \
  INSPECT_FLOAT(name "y", var.y);

#define INSPECT_COLOR(name, var)                                               \
  INSPECT_UCHAR(name "r", var.r);                                             \
  INSPECT_UCHAR(name "g", var.g);                                             \
  INSPECT_UCHAR(name "b", var.b);                                             \
  INSPECT_UCHAR(name "a", var.a);

#endif // IMACROS_H
