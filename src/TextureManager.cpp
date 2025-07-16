#include "TextureManager.h"

#include <cstring>
#include <SDL2/SDL_image.h>
#include <iostream>

bool TextureManager::loadTextures() {
    if (!this->loadFromFile("./../assets/textures/wall_brick.png", 0)) {
        return false;
    }
    if (!this->loadFromFile("./../assets/textures/wall_stones.png", 1)) {
        return false;
    }
    return true;
}

bool TextureManager::loadFromFile(const std::string &path, int textureIndex) {
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image " << path << std::endl;
        return false;
    }

    SDL_Surface *formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_FreeSurface(surface);
    if (!formatted) {
        std::cerr << "Failed to convert image " << path << std::endl;
        return false;
    }

    textureWidth = formatted->w;
    textureHeight = formatted->h;

    std::vector<uint32_t> pixels(textureWidth * textureHeight);
    std::memcpy(pixels.data(), formatted->pixels, textureWidth * textureHeight * sizeof(uint32_t));

    textures.push_back(std::move(pixels));
    SDL_FreeSurface(formatted);
    return true;
}

const std::vector<uint32_t> &TextureManager::getTexture(int index) const {
    return textures.at(index);
}

int TextureManager::getTextureWidth() const {
    return textureWidth;
}

int TextureManager::getTextureHeight() const {
    return textureHeight;
}






