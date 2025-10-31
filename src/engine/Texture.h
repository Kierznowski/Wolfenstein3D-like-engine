#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>


class Texture {
public:
    explicit Texture(const std::string& path);
    ~Texture();

    // disabled copying of Texture objects
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    int getWidth() const noexcept {
        return width;
    }

    int getHeight() const noexcept {
        return height;
    }

    uint32_t getPixel(int x, int y) const noexcept;

private:
    SDL_Surface* surface{nullptr};
    int width{0};
    int height{0};
    std::vector<uint32_t> pixels;
};

#endif // TEXTURE_H