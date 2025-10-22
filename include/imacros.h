#ifndef IMACROS_H
#define IMACROS_H

#include "iarray.h"
#include "inspector.h"
#include <string>

inline void inspect(Inspector &ins, std::string name, int &var) {
  ins.AddItem(
      name, [&](double delta) { var += static_cast<int>(delta); },
      [&]() { return std::to_string(var); });
}

inline void inspect(Inspector &ins, std::string name, float &var) {
  ins.AddItem(
      name, [&](double delta) { var += delta; },
      [&]() { return std::to_string(var); });
}

inline void inspect(Inspector &ins, std::string name, unsigned char &var) {
  ins.AddItem(
      name,
      [&](double delta) {
        var = std::clamp(var + static_cast<int>(delta), 0, 255);
      },
      [&]() { return std::to_string(var); });
}

inline void inspect(Inspector &ins, std::string name, Vector2 &var) {
  std::string dot = name.empty() ? "" : ".";
  inspect(ins, name + dot + "x", var.x);
  inspect(ins, name + dot + "y", var.y);
}

inline void inspect(Inspector &ins, std::string name, Color &var) {
  std::string dot = name.empty() ? "" : ".";
  inspect(ins, name + dot + "r", var.r);
  inspect(ins, name + dot + "g", var.g);
  inspect(ins, name + dot + "b", var.b);
  inspect(ins, name + dot + "a", var.a);
}

template <typename T>
void inspect(Inspector &ins, std::string name, iarray<T> &iarr) {
  ins.AddItem(name, iarr.change, iarr.str);
}

#endif // IMACROS_H
