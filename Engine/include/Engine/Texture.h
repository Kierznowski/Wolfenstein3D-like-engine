#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    int getWidth() const noexcept {
        return width;
    }

    int getHeight() const noexcept {
        return height;
    }

    uint32_t getPixel(int x, int y) const noexcept;

    const SDL_Surface* getSurface() const noexcept {
        return surface;
    }

private:
    SDL_Surface* surface{nullptr};
    int width{0};
    int height{0};
    std::vector<uint32_t> pixels;
};