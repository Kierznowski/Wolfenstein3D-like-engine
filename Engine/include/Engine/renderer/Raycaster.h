#pragma once

#include <memory>
#include <vector>

#include "../Map.h"
#include "../Texture.h"
#include "../entity/Entity.h"
#include "../utils/Viewport.h"

struct SpriteInstance;
class GameObject;

class Raycaster {
public:
    Raycaster(std::vector<uint32_t>& framebuffer,
        const Viewport& viewport,
        const Map& wallMap,
        const Map& floorMap,
        const Map& ceilingMap,
        const std::vector<std::unique_ptr<Texture>>& walls,
        const std::vector<std::unique_ptr<Texture>>& floorAndCeilingTex,
        const std::vector<std::unique_ptr<Entity>>& entities
        );

    void render(const Player& player) const;

private:
    std::vector<uint32_t>& framebuffer;
    const Viewport& viewport;
    const int windowWidth;
    const int windowHeight;
    const int offsetX;
    const int offsetY;
    const Map& wallMap;
    const Map& floorMap;
    const Map& ceilingMap;
    const std::vector<std::unique_ptr<Texture>>& wallTextures;
    const std::vector<std::unique_ptr<Texture>>& floorAndCeilingTextures;
    const std::vector<std::unique_ptr<Entity>>& entities_;

    void renderFloorAndCeiling(const Player& player) const;
    void renderWalls(const Player& player, std::vector<double>& zBuffer) const;
    void renderSprites(const Player& player, const std::vector<double>& zBuffer) const;
};
