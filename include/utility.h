#ifndef UTILITY_H
#define UTILITY_H

#include <raylib.h>
#include <string>
#include <typeindex>
#include <cxxabi.h>

inline std::string demangle(const std::type_index &t) {
  int status = 0;
  char *demangled = abi::__cxa_demangle(t.name(), nullptr, nullptr, &status);
  std::string result = (status == 0 && demangled) ? demangled : t.name();
  free(demangled);
  return result;
}

inline void DrawTextOutlined(const char *text, int posX, int posY, int fontSize,
                      Color textColor, Color outlineColor, int thickness) {
  for (int dx = -thickness; dx <= thickness; dx++) {
    for (int dy = -thickness; dy <= thickness; dy++) {
      if (dx == 0 && dy == 0)
        continue;
      DrawText(text, posX + dx, posY + dy, fontSize, outlineColor);
    }
  }

  DrawText(text, posX, posY, fontSize, textColor);
}

#endif // UTILITY_H
