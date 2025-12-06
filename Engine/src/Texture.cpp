#include "Engine/Texture.h"

#include <cstring>
#include <iostream>

Texture::Texture(const std::string &path) {
    SDL_Surface* loaded = SDL_LoadBMP(path.c_str());
    if (!loaded) {
        std::cerr << "Failed to load texture: " << path << " - " << SDL_GetError() << std::endl;
        return;
    }

    // convert bitmap to argb8888 format
    SDL_Surface* formatted = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_ARGB8888, 0);
    SDL_FreeSurface(loaded);
    if (!formatted) {
        std::cerr << "SDL_FreeSurface failed: " << SDL_GetError() << std::endl;
        return;
    }

    // copy pixels to pure c++ vector
    width = formatted->w;
    height = formatted->h;
    pixels.resize(width * height);
    std::memcpy(pixels.data(), formatted->pixels, width * height * sizeof(uint32_t));
    SDL_FreeSurface(formatted);

    std::cout << "Loaded texture: " << path << std::endl;
}

Texture::~Texture() = default;

uint32_t Texture::getPixel(int x, int y) const noexcept {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        return 0xFFFF00FF;
    }
    return pixels[y * width + x];
}

