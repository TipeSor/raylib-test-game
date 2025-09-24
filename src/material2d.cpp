#include "material2d.h"
#include <raylib.h>
#include <vector>

Material2D::Material2D(Shader shader, const std::vector<Texture> &textures)
    : shader(shader) {
  if (textures.size() == 0) {
    TraceLog(LOG_ERROR, "No Textures provided");
    return;
  }

  baseTexture = textures[0];

  for (size_t i = 1; i < textures.size(); ++i) {
    const std::string uniformName = "texture" + std::to_string(i);
    int texLoc = GetShaderLocation(shader, uniformName.c_str());
    if (texLoc == -1) {
      TraceLog(LOG_WARNING, "Shader uniform '%s' not found.",
               uniformName.c_str());
      break;
    }
    textureBindings[texLoc] = textures[i];
  }
}

void Material2D::SetTexture(int index, Texture texture) {
  std::string uniformName = "texture" + std::to_string(index);
  int loc = GetShaderLocation(shader, uniformName.c_str());
  if (loc != -1) {
    textureBindings[loc] = texture;
  }
}

void Material2D::SetShaderValue(const std::string &name, const void *value,
                                int type) {
  int locIndex = GetShaderLocation(shader, name.c_str());
  ::SetShaderValue(shader, locIndex, value, type);
}

void DrawMaterial2D(const Material2D &mat, int posX, int posY, Color tint) {
  BeginShaderMode(mat.shader);
  for (auto &kvp : mat.textureBindings) {
    SetShaderValueTexture(mat.shader, kvp.first, kvp.second);
  }
  DrawTexture(mat.baseTexture, posX, posY, tint);
  EndShaderMode();
}

void DrawMaterial2DPro(const Material2D &mat, Rectangle source, Rectangle dest,
                       Vector2 origin, float rotation, Color tint) {
  BeginShaderMode(mat.shader);
  for (auto &kvp : mat.textureBindings) {
    SetShaderValueTexture(mat.shader, kvp.first, kvp.second);
  }
  DrawTexturePro(mat.baseTexture, source, dest, origin, rotation, tint);
  EndShaderMode();
}
