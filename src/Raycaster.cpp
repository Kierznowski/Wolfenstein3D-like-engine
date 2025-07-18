#include "Raycaster.h"

#include <cmath>

Raycaster::Raycaster(Renderer &renderer, int screenWidth, int screenHeight)
    : renderer(&renderer), screenWidth(screenWidth), screenHeight(screenHeight) {
}

void Raycaster::setTextureManager(TextureManager& textureManager) {
    this->textureManager = &textureManager;
    this->textureWidth = textureManager.getTextureWidth();
    this->textureHeight = textureManager.getTextureHeight();
}

void Raycaster::setMap(const Map& map) {
    this->map = map;
}

void Raycaster::setPlayer(const Player& player) {
    this->player = player;
}

void Raycaster::setSprites(const std::vector<Sprite> &sprites) {
    this->sprites = sprites;
}


void Raycaster::renderFrame() {
    zBuffer.resize(screenWidth);

    for (int y = screenHeight / 2; y < screenHeight; y++) {
        castRayRow(y);
    }

    for (int x = 0; x < screenWidth; x++) {
        castRayColumn(x);
    }

    renderSprites();
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
    const int textureWidth = textureManager->getTextureWidth();
    const int textureHeight = textureManager->getTextureHeight();

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

    const double step = 1.0 * textureHeight / lineHeight;
    double texPos = (drawStart - screenHeight / 2.0 + lineHeight / 2.0) * step;

    for (int y = drawStart; y < drawEnd; y++) {
        const int texY = static_cast<int>(texPos) & (textureHeight - 1);
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

    zBuffer[x] = perpWallDist;
}

void Raycaster::castRayRow(int y) {
    // ray direction for the leftmost ray (x = 0) and the rightmost ray (x = width).
    // We need to add or subtract plane vector from player direction vector.
    const double rayDirX0 = player.dirX - player.planeX;
    const double rayDirY0 = player.dirY - player.planeY;
    const double rayDirX1 = player.dirX + player.planeX;
    const double rayDirY1 = player.dirY + player.planeY;

    // distance of the row from the middle of the screen
    const int p = y - screenHeight / 2;
    const double posZ = 0.5 * screenHeight;
    // distance from the camera plane to the row. For row near the middle -> infinity. For bottom row -> 1;
    const double rowDistance = posZ / p;

    const double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / screenWidth;
    const double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / screenWidth;

    // coordinates of the leftmost column
    double floorX = player.posX + rowDistance * rayDirX0;
    double floorY = player.posY + rowDistance * rayDirY0;

    for (int x = 0; x < screenWidth; x++) {
        // map tile coordinates
        const int cellX = static_cast<int>(floorX);
        const int cellY = static_cast<int>(floorY);
        if (cellX != oldCellX || cellY != oldCellY) {
            const int floorType = map.getFloorType(floorX, floorY);
            floorTextureCache = textureManager->getTexture(floorType);
            const int ceilingType = map.getCeilingType(cellX, cellY);
            ceilingTextureCache = textureManager->getTexture(ceilingType);
        }
        oldCellX = cellX;
        oldCellY = cellY;

        // texture coordinates
        const int texX = static_cast<int>(textureWidth * (floorX - cellX)) & (textureWidth - 1);
        const int texY = static_cast<int>(textureHeight * (floorY - cellY)) & (textureHeight - 1);

        const uint32_t floorColor = floorTextureCache[textureWidth * texY + texX];
        const uint32_t ceilingColor = ceilingTextureCache[textureWidth * texY + texX];
        renderer->putPixel(x, y, floorColor);
        renderer->putPixel(x, screenHeight - y - 1, ceilingColor);

        floorX += floorStepX;
        floorY += floorStepY;
    }
}

void Raycaster::renderSprites() {
    // storing sprites sorted by distance from player
    std::vector<std::pair<double, Sprite>> sortedSprites;
    for (const Sprite& sprite : sprites) {
        double dx = sprite.x - player.posX;
        double dy = sprite.y - player.posY;
        double distSq = dx * dx + dy * dy;
        sortedSprites.emplace_back(distSq, sprite);
    }
    std::sort(sortedSprites.begin(), sortedSprites.end(),
        [](const std::pair<double, Sprite>& a, const std::pair<double, Sprite>& b) {
            return a.first > b.first;
        });

    for (const auto& [distance, sprite] : sortedSprites) {
        double spriteX = sprite.x - player.posX;
        double spriteY = sprite.y - player.posY;

        // Inverse camera matrix transformation
        double invDet = 1.0 / (player.planeX * player.dirY - player.dirX * player.planeY);
        double transformX = invDet * (player.dirY * spriteX - player.dirX * spriteY);
        double transformY = invDet * (-player.planeY * spriteX + player.planeX * spriteY);

        // position on screen (left / right)
        int spriteScreenX = static_cast<int>((screenWidth / 2) * (1 + transformX / transformY));

        const auto& texture = textureManager->getTexture(sprite.textureIndex);
        int texWidth = textureManager->getTextureWidth();
        int texHeight = textureManager->getTextureHeight();

        // scale sprite height on the screen
        int spriteHeight = std::abs(static_cast<int>(screenHeight / transformY));
        int drawStartY = -spriteHeight / 2 + screenHeight / 2;
        if (drawStartY < 0) drawStartY = 0;
        int drawEndY = spriteHeight / 2 + screenHeight / 2;
        if (drawEndY >= screenHeight) drawEndY = screenHeight - 1;

        // scale sprite width on screen
        int spriteWidth = std::abs(static_cast<int>(screenHeight / transformY));
        int drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        int drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= screenWidth) drawEndX = screenWidth - 1;

        // drawing each pixel column
        for (int stripe = drawStartX; stripe < drawEndX; stripe++) {
            int texX = static_cast<int>(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
            if (transformY > 0 && stripe >= 0 && stripe < screenWidth && transformY < zBuffer[stripe]) {
                for (int y = drawStartY; y < drawEndY; y++) {
                    int d = (y) * 256 - screenHeight * 128 + spriteHeight * 128;
                    int texY = ((d * texHeight) / spriteHeight) / 256;

                    uint32_t color = texture[texY * texWidth + texX];
                    // check if isn't transparent
                    if ((color & 0x00FFFFFF) != 0) {
                        renderer->putPixel(stripe, y, color);
                    }
                }
            }
        }


    }

}