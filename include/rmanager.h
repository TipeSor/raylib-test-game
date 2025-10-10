#ifndef RMANAGER_H
#define RMANAGER_H

#include <memory>
#include <raylib.h>
#include <string>
#include <typeindex>
#include <unordered_map>

#define DEFINE_RESOURCE_TRAITS(Type, LoadFunc, UnloadFunc)                     \
  template <> struct ResourceTraits<Type> {                                    \
    static Type load(const std::string &path) {                                \
      return LoadFunc(path.c_str());                                           \
    }                                                                          \
    static void unload(Type &t) { UnloadFunc(t); }                             \
  };

template <typename T> struct ResourceTraits;

inline Shader LoadShaderFromPath(const char *fsFileName) {
  return LoadShader(0, fsFileName);
}

DEFINE_RESOURCE_TRAITS(Shader, LoadShaderFromPath, UnloadShader)
DEFINE_RESOURCE_TRAITS(Image, LoadImage, UnloadImage)
DEFINE_RESOURCE_TRAITS(Texture, LoadTexture, UnloadTexture)
DEFINE_RESOURCE_TRAITS(Sound, LoadSound, UnloadSound)

class ResourceManager {
public:
  template <typename T> T &load(const std::string &path) {
    auto &cache = getCache<T>();
    auto [it, inserter] =
        cache.try_emplace(path, ResourceTraits<T>::load(path));
    return it->second.value;
  }

  template <typename T> T &force_load(const std::string &path) {
    auto &cache = getCache<T>();
    auto it = cache.find(path);
    if (it != cache.end()) {
      ResourceTraits<T>::unload(it->second.value);
      it->second.value = ResourceTraits<T>::load(path);
      return it->second.value;
    }
    return load<T>(path);
  }

  template <typename T> void unload(T &value) {
    auto &cache = getCache<T>();
    for (auto it = cache.begin(); it != cache.end(); ++it) {
      if (&it->second.value == &value) {
        cache.erase(it);
        return;
      }
    }
  }

  template <typename T> void unload(const std::string &path) {
    auto &cache = getCache<T>();
    auto it = cache.find(path);
    if (it != cache.end()) {
      cache.erase(it);
    }
  }

  void unloadAll() { caches.clear(); }

private:
  template <typename T> struct ResourceHandle {
    T value;

    ResourceHandle(T &&v) : value(std::move(v)) {}
    ResourceHandle(const ResourceHandle &) = delete;
    ResourceHandle &operator=(const ResourceHandle &) = delete;

    ~ResourceHandle() { ResourceTraits<T>::unload(value); }
  };

  struct ICache {
    virtual ~ICache() = default;
  };

  template <typename T> struct Cache : ICache {
    std::unordered_map<std::string, ResourceHandle<T>> data;
  };

  std::unordered_map<std::type_index, std::unique_ptr<ICache>> caches;

  template <typename T> auto &getCache() {
    auto it = caches.find(typeid(T));
    if (it == caches.end()) {
      it = caches.emplace(typeid(T), std::make_unique<Cache<T>>()).first;
    }
    return static_cast<Cache<T> *>(it->second.get())->data;
  }
};

#endif // RMANAGER_H
