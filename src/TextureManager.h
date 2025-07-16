#pragma once
#include <vector>
#include <string>
#include <SDL_render.h>

class TextureManager {
public:
    bool loadTextures();
    const std::vector<uint32_t>& getTexture(int index) const;
    int getTextureWidth() const;
    int getTextureHeight() const;

private:
    bool loadFromFile(const std::string& path, int textureIndex);

    std::vector<std::vector<uint32_t>> textures;
    int textureWidth = 0;
    int textureHeight = 0;
};
