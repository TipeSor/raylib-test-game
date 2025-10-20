#ifndef MATERIAL2D_H
#define MATERIAL2D_H

#include <functional>
#include <memory>
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <utility>

class Material2D {
public:
  Material2D(Shader &shader, Texture &texture)
      : shader(shader), texture(texture) {}

  Material2D(const Material2D &) = delete;
  Material2D &operator=(const Material2D &) = delete;
  Material2D(Material2D &&) = default;
  Material2D &operator=(Material2D &&) = delete;

  template <typename T>
  void bind(const char *name, std::function<T()> value, int uniformType) {
    int loc = ::GetShaderLocation(shader, name);
    auto wrapper_func = [v = std::move(value),
                         storage = std::make_shared<T>()]() mutable -> void * {
      *storage = v();
      return storage.get();
    };
    uniforms.try_emplace(std::string(name),
                         (Uniform){loc, std::move(wrapper_func), uniformType});
  }

  template <typename T> void bind(const char *name, T &value, int uniformType) {
    int loc = ::GetShaderLocation(shader, name);
    uniforms.emplace(
        std::string(name),
        Uniform{loc, [&value]() -> void * { return &value; }, uniformType});
  }

  void unbind(const char *name) { uniforms.erase(name); }

  void reload() {
    for (auto &[name, data] : uniforms) {
      data.loc = GetShaderLocation(shader, name.c_str());
    }
  }

  void update() {
    for (auto &[name, data] : uniforms) {
      if (data.loc == -1)
        continue;
      SetShaderValue(shader, data.loc, data.value(), data.uniformType);
    }
  }

  void DrawPro(Rectangle source, Rectangle dest, Vector2 origin, float rotation,
               Color tint) {
    BeginShaderMode(shader);
    update();
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    EndShaderMode();
  }

  Shader &shader;
  Texture &texture;

private:
  struct Uniform {
    int loc;
    std::function<void *()> value;
    int uniformType;
  };
  std::unordered_map<std::string, Uniform> uniforms;
};

#endif // MATERIAL2D_H
