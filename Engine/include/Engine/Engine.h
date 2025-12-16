#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

#include "Map.h"
#include "Raycaster.h"
#include "Renderer.h"
#include "Entity/Sprite/Sprite.h"
#include "Texture.h"
#include "GameState.h"
#include "Command/CommandQueue.h"
#include "Entity/Entity.h"

class Engine {
public:
    Engine() = default ;
    ~Engine() = default;

    void setWindowSize(const int width, const int height) {
        windowWidth = width;
        windowHeight = height;
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
    void loadEntity(std::unique_ptr<Entity> entity) {
        entities_.push_back(std::move(entity));
    }

    void render() const;
    void loadWallTexture(const std::string& path);
    void loadFloorCeilingTexture(const std::string& path);
    void loadWallMap(const std::string& path);
    void loadFloorMap(const std::string& path);
    void loadCeilingMap(const std::string& path);
    std::shared_ptr<SpriteModel> loadSpriteModel(const std::string& id, const std::string& texturePath);
    void setState(std::unique_ptr<GameState> gameState);

    std::unordered_map<std::string, std::shared_ptr<SpriteModel>> spriteModels;

private:
    int windowWidth {800};
    int windowHeight {600};
    std::string gameTitle {"2.5DEngine"};
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Raycaster> raycaster;
    CommandQueue commandQueue;

    void initPlayer() const;
    void initRaycaster();
    void initRenderer();

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
};
