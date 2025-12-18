#include "../include/Engine/renderer/Renderer.h"
#include "../include/Engine/utils/Assets.h"
#include "Engine/utils/Viewport.h"

#include <cstring>
#include <stdexcept>
#include <SDL2/SDL_ttf.h>


Renderer::Renderer(const int width, const int height, const std::string &title)
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

    if (TTF_Init() < 0) {
        throw std::runtime_error(SDL_GetError());
    }

    font = TTF_OpenFont(Assets::path("/fonts/BrunoAceSC-Regular.ttf").c_str(), 20);
    if (!font) {
        throw std::runtime_error(SDL_GetError());
    }
}

Renderer::~Renderer() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::clear() {
    std::fill(framebuffer.begin(), framebuffer.end(), 0xFF000000);
}

void Renderer::clearViewport(const Viewport &viewport, const uint32_t color) {
    for (int y = viewport.y; y < viewport.y + viewport.height; y++) {
        for (int x = viewport.x; x < viewport.x + viewport.width; x++) {
            framebuffer[y * windowWidth + x] = color;
        }
    }
}

void Renderer::presentFrame() const {
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

void Renderer::drawText(const int posX, const int posY, const std::string& text) {
    constexpr SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if (!textSurface) {
        throw std::runtime_error(SDL_GetError());
    }

    if (textSurface->format->format != SDL_PIXELFORMAT_ARGB8888) {
        SDL_Surface* converted = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_ARGB8888, 0);
        SDL_FreeSurface(textSurface);
        textSurface = converted;
        if (!textSurface) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    const uint32_t* pixels = static_cast<uint32_t*>(textSurface->pixels);

    for (int y = 0; y < textSurface->h; y++) {
        const int fbY = posY + y;
        if (fbY < 0 || fbY >= windowHeight) continue;

        for (int x = 0; x < textSurface->w; x++) {
            const int fbX = posX + x;
            if (fbX < 0 || fbX >= windowWidth) continue;

            const uint32_t color = pixels[y * textSurface->w + x];
            framebuffer[fbY * windowWidth + fbX] = color;
        }
    }
    SDL_FreeSurface(textSurface);
}

void Renderer::drawTexturedRect(const int dstX, const int dstY, const int w, const int h, Texture* tex, const SDL_Rect& src) {
    const std::vector<uint32_t>& srcPixels = tex->getPixels();
    const int texW = tex->getWidth();

    for (int y = 0; y < h; y++) {
        const int fbY = dstY + y;
        if (fbY < 0 || fbY >= windowHeight) continue;

        const int srcY = src.y + (y * src.h) / h;

        for (int x = 0; x < w; x++) {
            const int fbX = dstX + x;
            if (fbX < 0 || fbX >= windowWidth) continue;

            const int srcX = src.x + (x * src.w) / w;
            const uint32_t color = srcPixels[srcY * texW + srcX];

            if ((color & 0x00FFFFFF) == 0) continue;
            framebuffer[fbY * windowWidth + fbX] = color;
        }
    }
}

void Renderer::drawPixel(int x, int y, uint32_t color) {
    framebuffer[y * windowWidth + x] = color;
}
