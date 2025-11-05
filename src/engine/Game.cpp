#include "Game.h"
#include "Raycaster.h"

#include <chrono>
#include <iostream>
#include <cmath>

constexpr double FIXED_TICK_TIME = 1.0 / 60.0;


Game::Game(int width, int height)
    : renderer(width, height, "2.5D Engine") {

    const int tmpMap[mapWidth][mapHeight] = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };


    worldMap.copyFrom(tmpMap);

    wallTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wall_stones.bmp"));
    wallTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wall_brick.bmp"));
    floorAndCeilingTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/floor_wood.bmp"));
    floorAndCeilingTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/floor_stones.bmp"));
    spriteTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wolftex/barrel.bmp"));
    spriteTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wolftex/pillar.bmp"));
    spriteTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wolftex/greenlight.bmp"));

    sprites.emplace_back(3.5, 3.5, spriteTextures[0].get(), 2.0, 128.0);
    sprites.emplace_back(4.5, 4.5, spriteTextures[1].get(), 1.0, 0.0);
    sprites.emplace_back(10.5, 10.5, spriteTextures[2].get(), 1.0, 0.0);

    raycaster = std::make_unique<Raycaster>(
        renderer.getFramebuffer(),
        renderer.getWidth(),
        renderer.getHeight(),
        worldMap,
        wallTextures,
        floorAndCeilingTextures,
        sprites);

    player.x = 22.0;
    player.y = 12.0;
    player.dirX = -1.0;
    player.dirY = 0.0;
    player.planeX = 0.0;
    player.planeY = 0.66; // FOV 66 deg
}


void Game::run() {
    using clock = std::chrono::high_resolution_clock;
    auto currentTime = clock::now();
    double accumulator = 0.0;

    int frames = 0;
    double fpsTimer = 0.0;
    double fps = 0.0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        auto newTime = clock::now();
        std::chrono::duration<double> frameTime = newTime - currentTime;
        currentTime = newTime;
        double deltaTime = frameTime.count();
        if (deltaTime > 0.25) deltaTime = 0.25;
        accumulator += deltaTime;

        while (accumulator >= FIXED_TICK_TIME) {
            update(FIXED_TICK_TIME);
            accumulator -= FIXED_TICK_TIME;
        }

        renderer.clear();
        raycaster->render(player);
        renderer.presentFrame();

        frames++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0) {
            fps = frames / fpsTimer;
            std::cout << "FPS: " << fps << std::endl;
            frames = 0;
            fpsTimer = 0.0;
        }
    }
}

void Game::update(double dt) {
    handleInput(dt);
}

void Game::handleInput(double dt) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    const double moveStep = player.moveSpeed * dt;
    const double rotationStep = player.rotationSpeed * dt;

    if (state[SDL_SCANCODE_W]) {
        double nextX = player.x + player.dirX * moveStep;
        double nextY = player.y + player.dirY * moveStep;

        if (worldMap.at(static_cast<int>(nextX), static_cast<int>(player.y)) == 0) {
            player.x = nextX;
        }
        if (worldMap.at(static_cast<int>(player.x), static_cast<int>(nextY)) == 0) {
            player.y = nextY;
        }
    }
    if (state[SDL_SCANCODE_S]) {
        double nextX = player.x - player.dirX * moveStep;
        double nextY = player.y - player.dirY * moveStep;

        if (worldMap.at(static_cast<int>(nextX), static_cast<int>(player.y)) == 0) {
            player.x = nextX;
        }
        if (worldMap.at(static_cast<int>(player.x), static_cast<int>(nextY)) == 0) {
            player.y = nextY;
        }
    }

    if (state[SDL_SCANCODE_A]) {
        const double oldDirX = player.dirX;
        player.dirX = player.dirX * std::cos(rotationStep) - player.dirY * std::sin(rotationStep);
        player.dirY = oldDirX * std::sin(rotationStep) + player.dirY * std::cos(rotationStep);

        const double oldPlaneX = player.planeX;
        player.planeX = player.planeX * std::cos(rotationStep) - player.planeY * std::sin(rotationStep);
        player.planeY = oldPlaneX * std::sin(rotationStep) + player.planeY * std::cos(rotationStep);
    }

    if (state[SDL_SCANCODE_D]) {
        const double oldDirX = player.dirX;
        player.dirX = player.dirX * std::cos(-rotationStep) - player.dirY * std::sin(-rotationStep);
        player.dirY = oldDirX * std::sin(-rotationStep) + player.dirY * std::cos(-rotationStep);

        const double oldPlaneX = player.planeX;
        player.planeX = player.planeX * std::cos(-rotationStep) - player.planeY * std::sin(-rotationStep);
        player.planeY = oldPlaneX * std::sin(-rotationStep) + player.planeY * std::cos(-rotationStep);
    }
}