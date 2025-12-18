#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../Texture.h"


struct Viewport;

class Renderer {
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    void clear();
    void clearViewport(const Viewport& viewport, uint32_t color);
    void presentFrame() const;
    void drawText(int posX, int posY, const std::string& text);

    int getWidth() const noexcept {
        return windowWidth;
    }
    int getHeight() const noexcept {
        return windowHeight;
    }
    std::vector<uint32_t>& getFramebuffer() noexcept {
        return framebuffer;
    }
    void drawTexturedRect(int dstX, int dstY, int w, int h, Texture* tex, const SDL_Rect& src);
    void drawPixel(int x, int y, uint32_t color);
private:
    int windowWidth;
    int windowHeight;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    std::vector<uint32_t> framebuffer;
    TTF_Font* font;
};
