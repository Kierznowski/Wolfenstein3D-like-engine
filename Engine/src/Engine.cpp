#include "Engine/Engine.h"

#include "Engine/Raycaster.h"

#include <chrono>
#include <iostream>
#include <queue>

constexpr double FIXED_TICK_TIME = 1.0 / 60.0;

void Engine::run() {
    std::cout << "Starting engine";

    if (currentState == nullptr) {
        throw std::logic_error("Engine state not set");
    }
    initPlayer();
    initRenderer();
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
            if (event.type == SDL_QUIT) running = false;
            currentState->handleInput(event, commandQueue);
        }
        currentState->handleRealtimeInput(commandQueue);

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

        currentState->render();

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

void Engine::update(const double dt) {
    for (const Command* command : commandQueue.getQueue()) {
        command->execute(*player_, dt);
    }
    commandQueue.clear();

    player_->update(dt);

    for (const std::unique_ptr<Entity>& entity : entities_) {
        entity->update(dt);
    }
}

void Engine::render() const {
    renderer->clear();
    raycaster->render(*player_);
    renderer->presentFrame();
}

void Engine::initPlayer() const {
    player_->entities = &entities_;
}

void Engine::initRenderer() {
    renderer = std::make_unique<Renderer>(windowWidth, windowHeight, gameTitle);
}

void Engine::initRaycaster() {
    raycaster = std::make_unique<Raycaster>(
        renderer->getFramebuffer(),
        windowWidth,
        windowHeight,
        wallMap,
        floorMap,
        ceilingMap,
        wallTextures,
        floorAndCeilingTextures,
        entities_);
}

void Engine::setState(std::unique_ptr<GameState> gameState) {
    currentState = std::move(gameState);
}

std::shared_ptr<SpriteModel> Engine::loadSpriteModel(const std::string& id, const std::string& texturePath) {
    auto texture = std::make_shared<Texture>(texturePath);
    auto model = std::make_shared<SpriteModel>(texture);
    spriteModels[id] = model;
    return model;
}

void Engine::loadWallTexture(const std::string& path) {
    wallTextures.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadFloorCeilingTexture(const std::string& path) {
    floorAndCeilingTextures.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadWallMap(const std::string& path) {
    wallMap.load(path);
    player_->wallMap = &wallMap;
}

void Engine::loadFloorMap(const std::string& path) {
    floorMap.load(path);
}

void Engine::loadCeilingMap(const std::string& path) {
    ceilingMap.load(path);
}