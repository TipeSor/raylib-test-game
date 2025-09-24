#ifndef MATERIAL2D_H
#define MATERIAL2D_H

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>

class Material2D {
public:
  Material2D(Shader shader, const std::vector<Texture> &textures);

  Shader shader;
  Texture baseTexture;
  std::unordered_map<int, Texture> textureBindings;

  void SetTexture(int index, Texture texture);
  void SetShaderValue(const std::string &name, const void *value, int type);
};

void DrawMaterial2D(const Material2D &mat, int posX, int posY, Color tint);
void DrawMaterial2DPro(const Material2D &mat, Rectangle source, Rectangle dest,
                       Vector2 origin, float rotation, Color tint);
#endif // MATERIAL2D_H
