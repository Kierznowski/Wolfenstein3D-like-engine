#include "Engine/Engine.h"
#include "../include/Engine/renderer/Raycaster.h"
#include "Engine/entity/DamageableEntity.h"

#include <chrono>
#include <iostream>
#include <queue>
#include <cassert>

constexpr double FIXED_TICK_TIME = 1.0 / 60.0;

void Engine::run() {
    assert(currentState_ && "Engine GameState not set before Engine::run()");
    assert(player_ && "Player not set before Engine::run()");
    assert(wallMap_ && "Map not initialized before Engine::run()");

    initPlayer();
    initRenderer();
    initRaycaster();

    if (hud_) hud_->init(*renderer_, *player_);

    using clock = std::chrono::high_resolution_clock;
    auto currentTime = clock::now();
    double accumulator = 0.0;

    int frames = 0;
    double fpsTimer = 0.0;
    double fps = 0.0;

    while (running_) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running_ = false;
            currentState_->handleInput(event, commandQueue_);
        }
        currentState_->handleRealtimeInput(commandQueue_);

        auto newTime = clock::now();
        std::chrono::duration<double> frameTime = newTime - currentTime;
        currentTime = newTime;
        double deltaTime = frameTime.count();
        if (deltaTime > 0.25) deltaTime = 0.25;
        accumulator += deltaTime;

        while (accumulator >= FIXED_TICK_TIME) {
            update(deltaTime);
            accumulator -= FIXED_TICK_TIME;
        }

        currentState_->render();

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
    for (const auto& command : commandQueue_.getQueue()) {
        command->execute(*player_, dt);
    }
    commandQueue_.clear();

    if (player_->consumeFireRequest()) {
        fireHitscan();
    }

    for (int i = 0; i < entities_.size(); i++) {
        if (!entities_[i]->alive) {
            entities_.erase(entities_.begin() + i);
            continue;
        }
        entities_[i]->update(dt);
    }
}

void Engine::render() const {
    renderer_->clear();
    raycaster_->render(*player_);
    if (hud_) {
        hud_->updateHealth(*renderer_, *player_);
        hud_->updateAmmo(*renderer_, *player_);
        hud_->drawMiniMap(*renderer_, *player_);
    }
    renderer_->presentFrame();
}

void Engine::initPlayer() const {
    player_->entities = &entities_;
}

void Engine::initRenderer() {
    renderer_ = std::make_unique<Renderer>(windowWidth_, windowHeight_, gameTitle_);
}

void Engine::initRaycaster() {
    raycaster_ = std::make_unique<Raycaster>(
        renderer_->getFramebuffer(),
        *gameplayArea_,
        *wallMap_,
        *floorMap_,
        *ceilingMap_,
        wallTextures_,
        floorAndCeilingTextures_,
        entities_);
}

void Engine::fireHitscan() const {
    constexpr double STEP = 0.05;
    constexpr double UPPER_BOUND = 100.0;
    double rayX = player_->x;
    double rayY = player_->y;

    for (double dist = 0.0; dist < UPPER_BOUND; dist += STEP) {
        rayX += player_->dirX * STEP;
        rayY += player_->dirY * STEP;

        if (wallMap_->at(static_cast<int>(rayX), static_cast<int>(rayY)) != 0) {
            return;
        }

        for (auto& entity : entities_) {

            const double dx = rayX - entity->position_.posX;
            const double dy = rayY - entity->position_.posY;
            const double r = entity->radius_;
            if (dx * dx + dy * dy < r * r) {
                if (entity->type_ == EntityType::DAMAGEABLE) {
                    dynamic_cast<DamageableEntity *>(entity.get())->onHit(*player_, 10);
                }
                return;
            }
        }
    }
}

std::shared_ptr<SpriteModel> Engine::loadSpriteModel(const std::string& id, const std::string& texturePath) {
    auto texture = std::make_shared<Texture>(texturePath);
    auto model = std::make_shared<SpriteModel>(texture);
    spriteModels[id] = model;
    return model;
}

void Engine::loadWallTexture(const std::string& path) {
    wallTextures_.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadFloorCeilingTexture(const std::string& path) {
    floorAndCeilingTextures_.emplace_back(std::make_unique<Texture>(path));
}

void Engine::loadWallMap(const std::string& path) const {
    wallMap_->load(path);
    player_->wallMap = wallMap_.get();
}

void Engine::loadFloorMap(const std::string& path) const {
    floorMap_->load(path);
}

void Engine::loadCeilingMap(const std::string& path) const {
    ceilingMap_->load(path);
}

void Engine::loadHud(std::unique_ptr<HUD> hud) {
    hud_ = std::move(hud);
}

void Engine::loadEntity(std::unique_ptr<Entity> entity) {
    entities_.emplace_back(std::move(entity));
}