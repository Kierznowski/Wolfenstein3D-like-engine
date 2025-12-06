#include "Engine/Engine.h"
#include "Engine/Raycaster.h"

#include <chrono>
#include <iostream>
#include <cmath>

constexpr double FIXED_TICK_TIME = 1.0 / 60.0;

Engine::Engine(int width, int height, const std::string& title)
    : renderer(width, height, title) {
}

void Engine::run() {
    std::cout << wallMap.getWidth() << ", " << wallMap.getHeight() << std::endl;
    initRaycaster();

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

void Engine::initRaycaster() {
    raycaster = std::make_unique<Raycaster>(
        renderer.getFramebuffer(),
        renderer.getWidth(),
        renderer.getHeight(),
        wallMap,
        floorMap,
        ceilingMap,
        wallTextures,
        floorAndCeilingTextures,
        sprites);
}

void Engine::loadWallTexture(const std::string& path) {
    wallTextures.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadFloorCeilingTexture(const std::string &path) {
    floorAndCeilingTextures.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadSpriteTexture(const std::string &path) {
    spriteTextures.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadWallMap(const std::string& path) {
    wallMap.load(path);
}

void Engine::loadFloorMap(const std::string& path) {
    floorMap.load(path);
}

void Engine::loadCeilingMap(const std::string& path) {
    ceilingMap.load(path);
}

void Engine::loadSprite(const Sprite& sprite) {
    this->sprites.emplace_back(sprite);
}

void Engine::setPlayer(const Player& newPlayer) {
    player = newPlayer;
}

void Engine::update(double dt) {
    handleInput(dt);
}

void Engine::handleInput(double dt) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    const double moveStep = player.moveSpeed * dt;
    const double rotationStep = player.rotationSpeed * dt;

    if (state[SDL_SCANCODE_W]) {
        double nextX = player.x + player.dirX * moveStep;
        double nextY = player.y + player.dirY * moveStep;

        if (wallMap.at(static_cast<int>(nextX), static_cast<int>(player.y)) == 0) {
            player.x = nextX;
        }
        if (wallMap.at(static_cast<int>(player.x), static_cast<int>(nextY)) == 0) {
            player.y = nextY;
        }
    }
    if (state[SDL_SCANCODE_S]) {
        double nextX = player.x - player.dirX * moveStep;
        double nextY = player.y - player.dirY * moveStep;

        if (wallMap.at(static_cast<int>(nextX), static_cast<int>(player.y)) == 0) {
            player.x = nextX;
        }
        if (wallMap.at(static_cast<int>(player.x), static_cast<int>(nextY)) == 0) {
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