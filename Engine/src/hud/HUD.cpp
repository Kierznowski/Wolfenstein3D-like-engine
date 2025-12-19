#include "../../include/Engine/hud/HUD.h"
#include "Engine/Player.h"

HUD::HUD(
    const int screenWidth,
    const int screenHeight,
    std::unique_ptr<BitmapFont> bitmapFont,
    std::unique_ptr<Texture> healthIcon,
    Map* wallmap)
        : hudArea(0, screenHeight - 120, screenWidth, 120),
        wallmap_(*wallmap),
        font_(std::move(bitmapFont)),
        healthIcon_(std::move(healthIcon))
{}

void HUD::init(Renderer& renderer, Player& player) const {
    renderer.clearViewport(hudArea, 0xFF121212);
    updateHealth(renderer, player);
    updateAmmo(renderer, player);
    drawMiniMap(renderer, player);
}

void HUD::updateHealth(Renderer &renderer, const Player& player) const {
    drawHealth(renderer, player.health());
}

void HUD::updateAmmo(Renderer &renderer, const Player& player) const {
    drawAmmo(renderer, player.ammo());
}

void HUD::drawHealth(Renderer& renderer, const int hp) const {
    constexpr int x = 20;
    const int y = hudArea.y + 30;
    renderer.drawTexturedRect(x, y, healthIcon_->getWidth(), healthIcon_->getHeight(),
        healthIcon_.get(), {0, 0, healthIcon_->getWidth(), healthIcon_->getHeight()});

    font_->drawText(renderer, x + healthIcon_->getWidth() + 20, y + 20, std::to_string(hp));
}

void HUD::drawMiniMap(Renderer& renderer, Player& player) const {
    constexpr int offsetX = 500;
    constexpr int offsetY = 20;
    constexpr int mapScale = 4;

    for (int y = 0; y < wallmap_.getHeight(); ++y) {
        for (int x = 0; x < wallmap_.getWidth(); ++x) {
            const uint32_t c = wallmap_.at(x,y) ? 0xFF770000 : 0xFF007700;
            renderer.drawPixel(
                hudArea.x + offsetX + y * mapScale,
                hudArea.y + offsetY + x * mapScale,
                c
            );
        }
    }
}

void HUD::drawAmmo(Renderer& renderer, const int ammo) const {
    constexpr int x = 350;
    const int y = hudArea.y + 50;

    font_->drawText(renderer, x, y, "AMMO");
    font_->drawText(renderer, x + 75, y, std::to_string(ammo));
}