#ifndef RMANAGER_H
#define RMANAGER_H

#include <raylib.h>
#include <string>
#include <unordered_map>

template <typename T> struct ResourceTraits;

template <> struct ResourceTraits<Shader> {
    static Shader load(const std::string& path) {
        return LoadShader(nullptr, path.c_str());
    }
    static void unload(Shader& s) { UnloadShader(s); }
};

template <> struct ResourceTraits<Image> {
    static Image load(const std::string& path) {
        return LoadImage(path.c_str());
    }
    static void unload(Image& i) { UnloadImage(i); }
};

template <> struct ResourceTraits<Texture> {
    static Texture load(const std::string& path) {
        return LoadTexture(path.c_str());
    }
    static void unload(Texture& t) { UnloadTexture(t); }
};

template <> struct ResourceTraits<Sound> {
    static Sound load(const std::string& path) {
        return LoadSound(path.c_str());
    }
    static void unload(Sound& s) { UnloadSound(s); }
};

template <typename T>
struct ResourceHandle {
    T value{};

    ResourceHandle() = default;
    explicit ResourceHandle(T v) : value(v) {}
    ~ResourceHandle() { ResourceTraits<T>::unload(value); }

    operator T&() { return value; }
    operator const T&() const { return value; }
};

class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager();

    template <typename T>
    ResourceHandle<T>& load(const std::string& path);

    template <typename T>
    void unload(T& handle);

    void unloadAll();

private:
    std::unordered_map<std::string, ResourceHandle<Shader>> shaderCache;
    std::unordered_map<std::string, ResourceHandle<Image>> imageCache;
    std::unordered_map<std::string, ResourceHandle<Texture>> textureCache;
    std::unordered_map<std::string, ResourceHandle<Sound>> soundCache;

    template <typename T>
    auto& getCache();
};

#endif // RMANAGER_H

