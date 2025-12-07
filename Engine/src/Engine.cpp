#include "Engine/Engine.h"
#include "Engine/Raycaster.h"

#include <chrono>
#include <iostream>
#include <queue>

constexpr double FIXED_TICK_TIME = 1.0 / 60.0;

Engine::Engine(int width, int height, const std::string& title)
    : renderer(width, height, title) {
}

void Engine::run() {
    std::cout << "Starting engine";
    if (currentState == nullptr) {
        throw std::logic_error("Engine state not set");
    }

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
            currentState->handleInput(event);
        }

        auto newTime = clock::now();
        std::chrono::duration<double> frameTime = newTime - currentTime;
        currentTime = newTime;
        double deltaTime = frameTime.count();
        if (deltaTime > 0.25) deltaTime = 0.25;
        accumulator += deltaTime;


        currentState->handleInput(commandQueue);

        while (accumulator >= FIXED_TICK_TIME) {
            update(FIXED_TICK_TIME);
            accumulator -= FIXED_TICK_TIME;
        }

        currentState->render(*this);
        if (currentState->isFinished) {
            currentState = std::move(currentState->nextState);
        }

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

void Engine::render() {
    renderer.clear();
    raycaster->render(player);
    renderer.presentFrame();
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

void Engine::update(double dt) {
    for (const auto& command : commandQueue.getQueue()) {
        command->execute(player, dt);
    }
    commandQueue.clear();
}

void Engine::setState(std::unique_ptr<GameState> gameState) {
    currentState = std::move(gameState);
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
    player.wallMap = &wallMap;
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
