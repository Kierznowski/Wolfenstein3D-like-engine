#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture() = default;

    int getWidth() const  {
        return width_;
    }

    int getHeight() const {
        return height_;
    }

    std::vector<uint32_t>& getPixels() {
        return pixels_;
    };

    const SDL_Surface* getSurface() const {
        return surface_;
    }

    uint32_t getPixel(int x, int y) const;

private:
    int width_{0};
    int height_{0};
    SDL_Surface* surface_{nullptr};
    std::vector<uint32_t> pixels_;
};