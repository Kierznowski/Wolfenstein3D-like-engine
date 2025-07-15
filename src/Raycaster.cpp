#include "Raycaster.h"

#include <cmath>

constexpr uint32_t FLOOR_COLOR = 0xFF444444;
constexpr uint32_t CEILING_COLOR = 0xFF8888FF;

Raycaster::Raycaster(Renderer &renderer, int screenWidth, int screenHeight)
    : renderer(&renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
}

void Raycaster::setTextureManager(TextureManager& textureManager) {
    this->textureManager = &textureManager;
}

void Raycaster::setMap(const Map& map) {
    this->map = map;
}

void Raycaster::setPlayer(const Player& player) {
    this->player = player;
}

void Raycaster::renderFrame() {
    for (int y = 0; y < screenHeight / 2; y++) {
        for (int x = 0; x < screenWidth; x++) {
            renderer->putPixel(x, y, CEILING_COLOR);
        }
    }

    for (int y = screenHeight / 2; y < screenHeight; y++) {
        for (int x = 0; x < screenWidth; x++) {
            renderer->putPixel(x, y, FLOOR_COLOR);
        }
    }

    for (int x = 0; x < screenWidth; x++) {
        castRayColumn(x);
    }
}

void Raycaster::castRayColumn(int x) {
    // Calculate cameraX in [-1..0..1] range
    const double cameraX = 2.0 * x / static_cast<double>(screenWidth) - 1.0;

    // Ray direction vector (player dir + camera plane shift)
    double rayDirX = player.dirX + player.planeX * cameraX;
    double rayDirY = player.dirY + player.planeY * cameraX;

    // Which square of the map we are in
    int mapX = static_cast<int>(player.posX);
    int mapY = static_cast<int>(player.posY);

    // Length of the ray between x and y sides of map squares
    double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1.0 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1.0 / rayDirY);
    double perpWallDist;

    double sideDistX;
    double sideDistY;
    int stepX;
    int stepY;

    // Calculate initial distance between player and first map square side
    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (player.posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
    }

    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (player.posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
    }

    // DDA - Digital Differential Analyzer
    bool hit = false;
    int side{0}; // 0 - x side, 1 - y side
    int wall_type{0};

    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        } else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }

        if (map.isWall(mapX, mapY)) {
            hit = true;
            wall_type = map.getWallType(mapX, mapY);
        }
    }

    if (side == 0) {
        perpWallDist = (sideDistX - deltaDistX);
    } else {
        perpWallDist = (sideDistY - deltaDistY);
    }

    const int lineHeight = static_cast<int>(screenHeight / perpWallDist);
    int drawStart = -lineHeight / 2 + screenHeight / 2;
    if (drawStart < 0) drawStart = 0;

    int drawEnd = lineHeight / 2 + screenHeight / 2;
    if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

    const auto& texture = textureManager->getTexture(wall_type);
    int textureWidth = textureManager->getTextureWidth();
    int textureHeight = textureManager->getTextureHeight();

    double wallX;
    if (side == 0) {
        wallX = player.posY + perpWallDist * rayDirY;
    } else {
        wallX = player.posX + perpWallDist * rayDirX;
    }
    wallX -= std::floor(wallX);

    int texX = static_cast<int>(wallX * textureWidth);
    if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0)) {
        texX = textureWidth - texX - 1;
    }

    double step = 1.0 * textureHeight / lineHeight;
    double texPos = (drawStart - screenHeight / 2.0 + lineHeight / 2.0) * step;

    for (int y = drawStart; y < drawEnd; y++) {
        int texY = static_cast<int>(texPos) & (textureHeight - 1);
        texPos += step;

        uint32_t color = texture[texY * textureWidth + texX];

        if (side == 1) {
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = (color >> 0) & 0xFF;
            r /= 2; g /= 2; b /= 2;
            color = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
        renderer->putPixel(x, y, color);
    }
}