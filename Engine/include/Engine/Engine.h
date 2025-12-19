#pragma once

#include "Map.h"
#include "renderer/Raycaster.h"
#include "renderer/Renderer.h"
#include "entity/Sprite/Sprite.h"
#include "Texture.h"
#include "GameState.h"
#include "hud/HUD.h"
#include "Command/CommandQueue.h"
#include "Engine/entity/Entity.h"

#include <chrono>
#include <memory>
#include <unordered_map>

class Engine {
public:
    Engine(const int windowWidth, const int windowHeight)
        : windowWidth_(windowWidth), windowHeight_(windowHeight)
    {};
    ~Engine() = default;

    int getWindowWidth() const {
        return windowWidth_;
    }

    int getWindowHeight() const {
        return windowHeight_;
    }

    Renderer* getRenderer() const {
        return renderer_.get();
    }

    Map* getWallMap() const {
        return wallMap_.get();
    }

    void setState(std::unique_ptr<GameState> gameState) {
        currentState_ = std::move(gameState);
    }

    void setWindowSize(const int width, const int height) {
        windowWidth_ = width;
        windowHeight_ = height;
    }

    void setPlayer(std::unique_ptr<Player> player) {
        player_ = std::move(player);
    }

    void render() const;
    void run();
    void stop() {
        running_ = false;
    }

    void loadEntity(std::unique_ptr<Entity> entity);
    void loadWallTexture(const std::string& path);
    void loadFloorCeilingTexture(const std::string& path);
    void loadWallMap(const std::string& path) const;
    void loadFloorMap(const std::string& path) const;
    void loadCeilingMap(const std::string& path) const;
    std::shared_ptr<SpriteModel> loadSpriteModel(const std::string& id, const std::string& texturePath);
    void loadHud(std::unique_ptr<HUD> hud);

    std::unordered_map<std::string, std::shared_ptr<SpriteModel>> spriteModels;

private:
    std::string gameTitle_ {"2.5DEngine"};
    const int HUD_HEIGHT_ {120};
    int windowWidth_ {800};
    int windowHeight_ {600};
    std::unique_ptr<HUD> hud_;
    std::unique_ptr<Viewport> gameplayArea_ = std::make_unique<Viewport>(0, 0, windowWidth_, windowHeight_ - HUD_HEIGHT_);

    bool running_ {true};

    CommandQueue commandQueue_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Raycaster> raycaster_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<GameState> currentState_;

    std::unique_ptr<Map> wallMap_ = std::make_unique<Map>();
    std::unique_ptr<Map> floorMap_ = std::make_unique<Map>();
    std::unique_ptr<Map> ceilingMap_ = std::make_unique<Map>();

    std::vector<std::unique_ptr<Texture>> wallTextures_;
    std::vector<std::unique_ptr<Texture>> floorAndCeilingTextures_;
    std::vector<std::unique_ptr<Entity>> entities_;

    void initPlayer() const;
    void initRaycaster();
    void initRenderer();
    void fireHitscan() const;
    void update(double dt);
};
