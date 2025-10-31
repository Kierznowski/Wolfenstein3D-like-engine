#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Renderer {
public:
    Renderer(int width, int height, const std::string& title);

    ~Renderer();

    void clear(uint32_t color = 0x000000FF);
    void presentFrame(const std::vector<uint32_t>& framebuffer);

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
};
#endif // RENDERER_H