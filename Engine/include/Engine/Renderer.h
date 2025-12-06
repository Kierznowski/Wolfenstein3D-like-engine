#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Renderer {
public:
    Renderer(int width, int height, const std::string& title);
    ~Renderer();

    void clear();
    void presentFrame();
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
private:
    int windowWidth;
    int windowHeight;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    std::vector<uint32_t> framebuffer;
    TTF_Font* font;
};
