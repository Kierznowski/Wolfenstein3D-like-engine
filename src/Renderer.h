#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cstdint>

class Renderer {
public:
    Renderer(SDL_Renderer *sdlRenderer, int width, int height);
    ~Renderer();

    void putPixel(int x, int y, uint32_t color);
    void present();
private:
    SDL_Renderer *sdlRenderer;
    SDL_Texture *texture;
    std::vector<uint32_t> framebuffer;
    int width;
    int height;
};