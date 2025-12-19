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
    width_ = formatted->w;
    height_ = formatted->h;
    pixels_.resize(width_ * height_ * sizeof(uint32_t));
    std::memcpy(pixels_.data(), formatted->pixels, width_ * height_ * sizeof(uint32_t));
    std::cout << pixels_[0] << std::endl;
    SDL_FreeSurface(formatted);

    std::cout << "Loaded texture: " << path << std::endl;
}

uint32_t Texture::getPixel(const int x, const int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return 0xFFFF00FF;
    }
    return pixels_[y * width_ + x];
}

