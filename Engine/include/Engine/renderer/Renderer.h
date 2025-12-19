#pragma once

#include "../Texture.h"

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Viewport;

class Renderer {
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    int getWidth() const noexcept {
        return windowWidth_;
    }

    int getHeight() const noexcept {
        return windowHeight_;
    }

    std::vector<uint32_t>& getFramebuffer() noexcept {
        return framebuffer_;
    }

    void clear();
    void clearViewport(const Viewport& viewport, uint32_t color);
    void presentFrame() const;
    void drawText(int posX, int posY, const std::string& text);
    void drawTexturedRect(int dstX, int dstY, int w, int h, Texture* tex, const SDL_Rect& src);
    void drawPixel(int x, int y, uint32_t color);
private:
    int windowWidth_;
    int windowHeight_;
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    std::vector<uint32_t> framebuffer_;
    TTF_Font* font_;
};
