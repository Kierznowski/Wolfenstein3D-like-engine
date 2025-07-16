#pragma once
#include "Renderer.h"
#include "Player.h"
#include "Map.h"
#include "TextureManager.h"

class Raycaster {
public:
    Raycaster(Renderer& renderer, int screenWidth, int screenHeight);

    void setTextureManager(TextureManager& textureManager);
    void setMap(const Map& map);
    void setPlayer(const Player& player);

    void renderFrame();
private:
    Renderer *renderer;
    TextureManager *textureManager;
    int screenWidth;
    int screenHeight;

    Player player;
    Map map;

    void castRayColumn(int x);
    void castRayRow(int y);
};
