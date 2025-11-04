#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <memory>
#include <vector>

#include "Map.h"
#include "Texture.h"
#include "Player.h"
#include "Sprite.h"

class Raycaster {
public:
    Raycaster(std::vector<uint32_t>& framebuffer,
        int width, int height,
        const Map& map,
        const std::vector<std::unique_ptr<Texture>>& walls,
        const std::vector<std::unique_ptr<Texture>>& floorAndCeilingTex,
        const std::vector<Sprite>& sprites);

    void render(const Player& player);

private:
    void renderFloorAndCeiling(const Player& player);
    void renderWalls(const Player& player, std::vector<double>& zBuffer);
    void renderSprites(const Player& player, const std::vector<double>& zBuffer);

    const std::vector<Sprite>& sprites;
    std::vector<uint32_t>& framebuffer;
    const int windowWidth;
    const int windowHeight;
    const Map& map;
    const std::vector<std::unique_ptr<Texture>>& wallTextures;
    const std::vector<std::unique_ptr<Texture>>& floorAndCeilingTextures;
};

#endif // RAYCASTER_H