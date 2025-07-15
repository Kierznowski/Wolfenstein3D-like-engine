#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "Renderer.h"
#include "Map.h"
#include "Player.h"
#include "Raycaster.h"
#include "TextureManager.h"

class Game {
public:
    Game();
    ~Game();

    void run();
private:
    void init();
    void handleEvents();
    void render();
    void cleanup();

    static constexpr int SCREEN_WIDTH = 800;
    static constexpr int SCREEN_HEIGHT = 640;

    SDL_Window *window = nullptr;
    SDL_Renderer *sdlRenderer = nullptr;

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Raycaster> raycaster;

    Map map;
    Player player;
    TextureManager textureManager;

    bool running = false;
};