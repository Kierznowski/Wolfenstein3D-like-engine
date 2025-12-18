#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

#include "Map.h"
#include "renderer/Raycaster.h"
#include "renderer/Renderer.h"
#include "entity/Sprite/Sprite.h"
#include "Texture.h"
#include "GameState.h"
#include "hud/HUD.h"
#include "Command/CommandQueue.h"
#include "Engine/entity/Entity.h"

class Engine {
public:
    Engine(const int windowWidth, const int windowHeight)
        : windowWidth(windowWidth), windowHeight(windowHeight)
    {};
    ~Engine() = default;

    void setWindowSize(const int width, const int height) {
        windowWidth = width;
        windowHeight = height;
    }

    int getWindowWidth() const {
        return windowWidth;
    }

    int getWindowHeight() const {
        return windowHeight;
    }

    Renderer* getRenderer() const {
        return renderer.get();
    }

    void setPlayer(Player* player) {
        player_ = player;
    }

    void run();
    void stop() {
        running = false;
    }

    Map& getWallMap() {
        return wallMap;
    }

    void render() const;

    void loadEntity(std::unique_ptr<Entity> entity) {
        entities_.push_back(std::move(entity));
    }
    void loadWallTexture(const std::string& path);
    void loadFloorCeilingTexture(const std::string& path);
    void loadWallMap(const std::string& path);
    void loadFloorMap(const std::string& path);
    void loadCeilingMap(const std::string& path);
    std::shared_ptr<SpriteModel> loadSpriteModel(const std::string& id, const std::string& texturePath);
    void setState(std::unique_ptr<GameState> gameState);
    void loadHud(std::unique_ptr<HUD> hud);

    std::unordered_map<std::string, std::shared_ptr<SpriteModel>> spriteModels;

private:
    const int HUD_HEIGHT {120};
    int windowWidth {800};
    int windowHeight {600};
    std::string gameTitle {"2.5DEngine"};
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Raycaster> raycaster;
    CommandQueue commandQueue;

    void initPlayer() const;
    void initRaycaster();
    void initRenderer();

    void fireHitscan() const;

    void update(double dt);

    Player* player_ = nullptr;

    std::unique_ptr<GameState> currentState;

    Map wallMap;
    Map floorMap;
    Map ceilingMap;

    std::vector<std::unique_ptr<Texture>> wallTextures;
    std::vector<std::unique_ptr<Texture>> floorAndCeilingTextures;
    std::vector<std::unique_ptr<Entity>> entities_;

    bool running {true};
    std::unique_ptr<HUD> hud_;
    std::unique_ptr<Viewport> gameplayArea_ = std::make_unique<Viewport>(0, 0, windowWidth, windowHeight - HUD_HEIGHT);
};
