#include "Renderer.h"

#include <cstring>
#include <stdexcept>

Renderer::Renderer(int width, int height, const std::string &title)
    : windowWidth(width), windowHeight(height), framebuffer(width * height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        0);

    if (!window) {
        throw std::runtime_error(SDL_GetError());
    }

    constexpr bool enableVSync = false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | (enableVSync ? SDL_RENDERER_PRESENTVSYNC : 0));
    if (!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowWidth, windowHeight);

    if (!texture) {
        throw std::runtime_error(SDL_GetError());
    }

    framebuffer.resize(windowWidth * windowHeight, 0);
}

Renderer::~Renderer() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::clear(const uint32_t color) {
    std::fill(framebuffer.begin(), framebuffer.end(), color);
}

void Renderer::presentFrame() {
    void* pixels {nullptr};
    int pitch;
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) == 0) {
        std::memcpy(pixels, framebuffer.data(), framebuffer.size() * sizeof(uint32_t));
        SDL_UnlockTexture(texture);
    }
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

