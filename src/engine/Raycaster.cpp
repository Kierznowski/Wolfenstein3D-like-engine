#include "Raycaster.h"
#include <vector>

Raycaster::Raycaster(std::vector<uint32_t>& framebuffer,
    const int width,
    const int height,
    const Map &map,
    const std::vector<std::unique_ptr<Texture>> &walls,
    const std::vector<std::unique_ptr<Texture> > &floorAndCeilingTex,
    const std::vector<Sprite> sprites) noexcept
    : framebuffer{framebuffer},
    windowWidth{width},
    windowHeight{height},
    map{map},
    wallTextures{walls},
    floorAndCeilingTextures{floorAndCeilingTex},
    sprites{sprites} {}

void Raycaster::render(const Player& player) {
    if (floorAndCeilingTextures.size() != 0) {
        renderFloorAndCeiling(player);
    } else {
        std::fill(framebuffer.begin(), framebuffer.end(), 0xFF444444);
    }
    renderWalls(player);
}

void Raycaster::renderFloorAndCeiling(const Player& player) {
    const double posX = player.x;
    const double posY = player.y;
    const double dirX = player.dirX;
    const double dirY = player.dirY;
    const double planeX = player.planeX;
    const double planeY = player.planeY;

    for (int y = windowHeight / 2; y < windowHeight; y++) {
        const double rayDirX0 = dirX - planeX;
        const double rayDirY0 = dirY - planeY;
        const double rayDirX1 = dirX + planeX;
        const double rayDirY1 = dirY + planeY;

        const int p = y - windowHeight / 2;
        const double posZ = windowHeight / 2.0; // middle of the screen
        const double rowDistance = posZ / p;

        const double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / windowWidth;
        const double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / windowWidth;

        double floorX = posX + rowDistance * rayDirX0;
        double floorY = posY + rowDistance * rayDirY0;

        for (int x = 0; x < windowWidth; x++) {
            const int cellX = static_cast<int>(floorX);
            const int cellY = static_cast<int>(floorY);

            const int tx = static_cast<int>(floorAndCeilingTextures[0]->getWidth() * (floorX - cellX)) & (floorAndCeilingTextures[0]->getWidth() - 1);
            const int ty = static_cast<int>(floorAndCeilingTextures[0]->getHeight() * (floorY - cellY)) & (floorAndCeilingTextures[0]->getHeight() - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            const uint32_t floorColor = floorAndCeilingTextures[0]->getPixel(tx, ty);
            framebuffer[y * windowWidth + x] = floorColor;

            const int ceilingY = windowHeight - y - 1;
            const uint32_t ceilColor = floorAndCeilingTextures[1]->getPixel(tx, ty);
            framebuffer[ceilingY * windowWidth + x] = ceilColor;
        }
    }
}

void Raycaster::renderWalls(const Player& player) {
    for (int x = 0; x < windowWidth; x++) {
        // calculate ray position
        const double cameraX = 2 * x / static_cast<double>(windowWidth) - 1;
        const double rayDirX = player.dirX + player.planeX * cameraX;
        const double rayDirY = player.dirY + player.planeY * cameraX;

        // integer part of ray position = tile coordinates
        // floating point value of ray position = actual position of the ray
        int mapX = static_cast<int>(player.x);
        int mapY = static_cast<int>(player.y);

        // calculating deltaDisX (distance ray have to travel to go through one cell from side to side in x-axis)
        // calculating deltaDisY (distance ray have to travel to go through exactly one cell from bottom to top [in y-axis])
        // It's from Pythagoras, and can be simplified to:
        const double deltaDistX = std::abs(1 / rayDirX);
        const double deltaDistY = std::abs(1 / rayDirY);
        // If language doesn't support Infinite (like C), we should check division by 0 and set result to e.g. 1e30

        // sideDistX, sideDistY, starting distance between player position and x, y walls of the tiles.
        double sideDistX;
        double sideDistY;

        // stepX and stepY tell us, if we should trace in positive direction or negative direction (-1 or +1)
        int stepX, stepY;
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (player.x - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - player.x) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (player.y - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - player.y) * deltaDistY;
        }

        // hit detection
        bool hit {false};
        int side {}; // 0 = X side of the wall hit, 1 = Y side hit
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

            if (mapX < 0 || mapX >= map.getWidth() || mapY < 0 || mapY >= map.getHeight()) {
                hit = true;
                break;
            }

            if (map.at(mapX, mapY) > 0) {
                hit = true;
            }
        }

        // we need to calculate perpendicular distance from the hit wall to the camera plane.
        // Euclidian distance will cause fisheye view
        // We just need to reverse one step of our tracing (sideDist - deltaDist)
        double perpWallDist {};
        if (side == 0) {
            perpWallDist = sideDistX - deltaDistX;
        } else {
            perpWallDist = sideDistY - deltaDistY;
        }

        // to get perspective we calculate height of the wall from proportions
        const int lineHeight = static_cast<int>(windowHeight / perpWallDist);
        int drawStart = -lineHeight / 2 + windowHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + windowHeight / 2;
        if (drawEnd >= windowHeight) drawEnd = windowHeight - 1;

        int texIndex = map.at(mapX, mapY) - 1;
        const Texture* tex = wallTextures[std::clamp(texIndex, 0, static_cast<int>(wallTextures.size()-1))].get();

        // calculating where the wall is hit. wallX is x coordinate of texture!
        // If side == 0 it's also x coordinate of wall on mapp, if side == 1 it's y coordinate of wall on map
        // We then subtract integer part to get relative position of hit on the tile, not absolute on the map
        double wallX;
        if (side == 0) {
            wallX = player.y + perpWallDist * rayDirY;
        } else {
            wallX = player.x + perpWallDist * rayDirX;
        }
        wallX -= std::floor(wallX);

        // we need to convert wallX to coordinate on the texture - texX.
        // WallX is fraction, so we multiply it by tex width. WallX does not change because we calculate vertical stripe
        int texX = static_cast<int>(wallX * static_cast<double>(tex->getWidth()));
        if (side == 0 && rayDirX > 0) texX = tex->getWidth() - texX - 1;
        if (side == 1 && rayDirY < 0) texX = tex->getWidth() - texX - 1;

        double step = 1.0 * tex->getHeight() / lineHeight;
        double texPos = (drawStart - windowHeight / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++) {
            int texY = static_cast<int>(texPos) & (tex->getHeight() - 1);
            texPos += step;
            uint32_t color = tex->getPixel(texX, texY);
            // making 1-side darker. Shifting by 1 bit divides color by 2. Then we set first bit of each byte to 0
            if (side == 1) color = (color >> 1) & 0x7F7F7F7F;
            framebuffer[y * windowWidth + x] = color;
        }
    }
}
