#pragma once

#include <memory>
#include <unordered_map>

#include "Map.h"
#include "Raycaster.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Texture.h"
#include "GameState.h"
#include "CommandQueue.h"

class Engine {
public:
    Engine(int width, int height, const std::string& title);
    ~Engine() = default;

    void run();

    Renderer& getRenderer() {
        return renderer;
    }

    void render();

    void loadWallTexture(const std::string& path);
    void loadFloorCeilingTexture(const std::string& path);

    void loadWallMap(const std::string& path);
    void loadFloorMap(const std::string& path);
    void loadCeilingMap(const std::string& path);

    std::shared_ptr<SpriteModel> loadSpriteModel(const std::string& id, const std::string& texturePath);
    void loadSprite(const Sprite& sprite);

    void setPlayer(const Player& player);
    std::unordered_map<std::string, std::shared_ptr<SpriteModel>> spriteModels;

    void setState(std::unique_ptr<GameState> gameState);

private:
    void initRaycaster();
    void update(double dt);

    CommandQueue commandQueue;

    std::unique_ptr<GameState> currentState;

    Renderer renderer;
    Player player;

    Map wallMap;
    Map floorMap;
    Map ceilingMap;

    std::vector<std::unique_ptr<Texture>> wallTextures;
    std::vector<std::unique_ptr<Texture>> floorAndCeilingTextures;
    std::vector<std::unique_ptr<Texture>> spriteTextures;
    std::vector<Sprite> sprites;

    std::unique_ptr<Raycaster> raycaster;

    bool running {true};
};