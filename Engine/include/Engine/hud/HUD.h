#pragma once
#include <memory>

#include "../renderer/Renderer.h"
#include "../utils/Viewport.h"
#include "Engine/utils/BitmapFont.h"

class Player;
class Map;

class HUD {
public:
    HUD(
        int screenWidth,
        int screenHeight,
        std::unique_ptr<BitmapFont> bitmapFont,
        std::unique_ptr<Texture> healthIcon,
        Map& wallmap);
    void updateHealth(Renderer &renderer, const Player &player) const;
    void updateAmmo(Renderer &renderer, const Player &player) const;
    void init(Renderer &renderer, Player &player);

    Viewport hudArea{};
private:
    Map& wallmap_;
    std::unique_ptr<BitmapFont> font_;
    std::unique_ptr<Texture> healthIcon_;

    void drawHealth(Renderer& renderer, int hp) const;
    void drawAmmo(Renderer& renderer, int ammo) const;
    void drawMiniMap(Renderer& renderer, Player& player) const;
};
