#include "Renderer.h"
#include <iostream>

Renderer::Renderer(SDL_Renderer *sdlRenderer, int width, int height)
    : sdlRenderer(sdlRenderer), width(width), height(height) {

    framebuffer.resize(width * height);
    texture = SDL_CreateTexture(sdlRenderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Could not create texture" << '\n';
        std::exit(EXIT_FAILURE);
    }
}

Renderer::~Renderer() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Renderer::clear(uint32_t color) {
    std::fill(framebuffer.begin(), framebuffer.end(), color);
}

void Renderer::putPixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        framebuffer[y * width + x] = color;
    }
}

void Renderer::present() {
    SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * sizeof(uint32_t));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, texture, nullptr, nullptr);
    SDL_RenderPresent(sdlRenderer);
}

