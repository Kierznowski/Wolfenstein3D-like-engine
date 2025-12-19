#pragma once

#include "../Map.h"
#include "../Texture.h"
#include "../entity/Entity.h"
#include "../utils/Viewport.h"

#include <memory>
#include <vector>

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
    std::vector<uint32_t>& framebuffer_;
    const Viewport& viewport_;
    const int windowWidth_;
    const int windowHeight_;
    const int offsetX_;
    const int offsetY_;
    const Map& wallMap_;
    const Map& floorMap_;
    const Map& ceilingMap_;
    const std::vector<std::unique_ptr<Texture>>& wallTextures_;
    const std::vector<std::unique_ptr<Texture>>& floorAndCeilingTextures_;
    const std::vector<std::unique_ptr<Entity>>& entities_;

    void renderFloorAndCeiling(const Player& player) const;
    void renderWalls(const Player& player, std::vector<double>& zBuffer) const;
    void renderSprites(const Player& player, const std::vector<double>& zBuffer) const;
};
