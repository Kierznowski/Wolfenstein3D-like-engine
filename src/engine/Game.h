#ifndef GAME_H
#define GAME_H
#include <memory>

#include "Map.h"
#include "Raycaster.h"
#include "Renderer.h"
#include "Sprite.h"
#include "Texture.h"

class Raycaster;

class Game {
public:
    Game(int width, int height);
    ~Game() = default;

    void run();

private:
    void update(double dt);
    void handleInput(double dt);

    static constexpr int mapWidth = 24;
    static constexpr int mapHeight = 24;

    Renderer renderer;
    Map worldMap;
    Player player;
    std::unique_ptr<Raycaster> raycaster;

    bool running {true};
    std::vector<std::unique_ptr<Texture>> wallTextures;
    std::vector<std::unique_ptr<Texture>> floorAndCeilingTextures;
};


#endif // GAME_H