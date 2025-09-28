#include "rmanager.h"

ResourceManager::~ResourceManager() { unloadAll(); }

void ResourceManager::unloadAll() {
  shaderCache.clear();
  imageCache.clear();
  textureCache.clear();
  soundCache.clear();
}

template <typename T>
ResourceHandle<T> &ResourceManager::load(const std::string &path) {
  auto &cache = getCache<T>();
  auto [it, inserted] = cache.try_emplace(path, ResourceTraits<T>::load(path));
  return it->second;
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

template <> auto &ResourceManager::getCache<Shader>() { return shaderCache; }
template <> auto &ResourceManager::getCache<Image>() { return imageCache; }
template <> auto &ResourceManager::getCache<Texture>() { return textureCache; }
template <> auto &ResourceManager::getCache<Sound>() { return soundCache; }

template ResourceHandle<Shader> &
ResourceManager::load<Shader>(const std::string &);
template ResourceHandle<Image> &
ResourceManager::load<Image>(const std::string &);
template ResourceHandle<Texture> &
ResourceManager::load<Texture>(const std::string &);
template ResourceHandle<Sound> &
ResourceManager::load<Sound>(const std::string &);

template void ResourceManager::unload<Shader>(Shader &);
template void ResourceManager::unload<Image>(Image &);
template void ResourceManager::unload<Texture>(Texture &);
template void ResourceManager::unload<Sound>(Sound &);
