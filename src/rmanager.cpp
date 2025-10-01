#include "rmanager.h"

ResourceManager::~ResourceManager() { unloadAll(); }

void ResourceManager::unloadAll() {
  shaderCache.clear();
  imageCache.clear();
  textureCache.clear();
  soundCache.clear();
}

template <typename T> T &ResourceManager::load(const std::string &path) {
  auto &cache = getCache<T>();
  auto [it, inserted] = cache.try_emplace(path, ResourceTraits<T>::load(path));
  return it->second;
}

template <typename T> T &ResourceManager::force_load(const std::string &path) {
  unload<T>(path);
  return load<T>(path);
}

template <typename T> void ResourceManager::unload(T &value) {
  auto &cache = getCache<T>();
  for (auto it = cache.begin(); it != cache.end(); ++it) {
    if (&it->second.value == &value) {
      cache.erase(it);
      return;
    }
  }
}

template <typename T> void ResourceManager::unload(const std::string &path) {
  auto &cache = getCache<T>();
  auto it = cache.find(path);
  if (it != cache.end()) {
    cache.erase(it);
  }
}

template <> auto &ResourceManager::getCache<Shader>() { return shaderCache; }
template <> auto &ResourceManager::getCache<Image>() { return imageCache; }
template <> auto &ResourceManager::getCache<Texture>() { return textureCache; }
template <> auto &ResourceManager::getCache<Sound>() { return soundCache; }

template Shader &ResourceManager::load<Shader>(const std::string &);
template Image &ResourceManager::load<Image>(const std::string &);
template Texture &ResourceManager::load<Texture>(const std::string &);
template Sound &ResourceManager::load<Sound>(const std::string &);

template Shader &ResourceManager::force_load<Shader>(const std::string &);
template Image &ResourceManager::force_load<Image>(const std::string &);
template Texture &ResourceManager::force_load<Texture>(const std::string &);
template Sound &ResourceManager::force_load<Sound>(const std::string &);

template void ResourceManager::unload<Shader>(Shader &);
template void ResourceManager::unload<Image>(Image &);
template void ResourceManager::unload<Texture>(Texture &);
template void ResourceManager::unload<Sound>(Sound &);

template void ResourceManager::unload<Shader>(const std::string &);
template void ResourceManager::unload<Image>(const std::string &);
template void ResourceManager::unload<Texture>(const std::string &);
template void ResourceManager::unload<Sound>(const std::string &);
