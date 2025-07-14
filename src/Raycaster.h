#pragma once
#include <vector>
#include <cstdint>
#include "Renderer.h"
#include "Player.h"
#include "Map.h"

class Raycaster {
public:
    Raycaster(Renderer& renderer, int screenWidth, int screenHeight);

    void setMap(const Map& map);
    void setPlayer(const Player& player);

    void renderFrame();
private:
    Renderer *renderer;
    int screenWidth;
    int screenHeight;

    Player player;
    Map map;

    void castRayColumn(int x);
};
