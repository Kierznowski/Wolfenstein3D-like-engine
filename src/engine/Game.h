#ifndef GAME_H
#define GAME_H
#include <memory>

#include "Renderer.h"
#include "Texture.h"

struct Player {
    double x{22.0};
    double y{12.0};
    double dirX{-1.0};
    double dirY{0.0};
    double planeX{0.0};
    double planeY{0.66};

    double moveSpeed{3.0}; // tiles per sec
    double rotationSpeed{2.0}; // rads per sec
};


class Game {
public:
    Game(int width, int height);
    void run();

private:
    void update(double dt);
    void floorCeilingCasting(int w, int h);
    void ddaRaycasting();
    void handleInput(double dt);

    Renderer renderer;
    bool running {true};

    static constexpr int mapWidth = 24;
    static constexpr int mapHeight = 24;
    int worldMap[mapWidth][mapHeight];

    Player player;

    std::vector<std::unique_ptr<Texture>> wallTextures;
    std::vector<std::unique_ptr<Texture>> floorTextures;
    std::vector<std::unique_ptr<Texture>> ceilingTextures;
};


#endif // GAME_H