#ifndef UTILITY_H
#define UTILITY_H

#include <cxxabi.h>
#include <raylib.h>
#include <string>
#include <typeindex>

inline std::string demangle(const std::type_index &t) {
  int status = 0;
  char *demangled = abi::__cxa_demangle(t.name(), nullptr, nullptr, &status);
  std::string result = (status == 0 && demangled) ? demangled : t.name();
  free(demangled);
  return result;
}

#endif // UTILITY_H
