#include "../../include/Engine/renderer/Raycaster.h"
#include "../../../Game/include/Game/Game.h"

#include <vector>

Raycaster::Raycaster(
    std::vector<uint32_t>& framebuffer,
    const Viewport& viewport,
    const Map &wallMap,
    const Map &floorMap,
    const Map &ceilingMap,
    const std::vector<std::unique_ptr<Texture>>& walls,
    const std::vector<std::unique_ptr<Texture>>& floorAndCeilingTex,
    const std::vector<std::unique_ptr<Entity>>& entities)
        : framebuffer_{framebuffer},
        viewport_(viewport),
        windowWidth_{viewport.width},
        windowHeight_{viewport.height},
        offsetX_{viewport.x},
        offsetY_{viewport.y},
        wallMap_{wallMap},
        floorMap_{floorMap},
        ceilingMap_{ceilingMap},
        wallTextures_{walls},
        floorAndCeilingTextures_{floorAndCeilingTex},
        entities_{entities} {
}

void Raycaster::render(const Player& player) const {
    std::vector<double> zBuffer(windowWidth_);
    renderFloorAndCeiling(player);
    renderWalls(player, zBuffer);
    renderSprites(player, zBuffer);
}

void Raycaster::renderFloorAndCeiling(const Player& player) const {
    const double posX = player.x;
    const double posY = player.y;
    const double dirX = player.dirX;
    const double dirY = player.dirY;
    const double planeX = player.planeX;
    const double planeY = player.planeY;

    for (int y = windowHeight_ / 2; y < windowHeight_; y++) {
        const double rayDirX0 = dirX - planeX;
        const double rayDirY0 = dirY - planeY;
        const double rayDirX1 = dirX + planeX;
        const double rayDirY1 = dirY + planeY;

        const int p = y - windowHeight_ / 2;
        const double posZ = windowHeight_ / 2.0; // middle of the screen
        const double rowDistance = posZ / p;

        const double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / windowWidth_;
        const double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / windowWidth_;

        double floorX = posX + rowDistance * rayDirX0;
        double floorY = posY + rowDistance * rayDirY0;

        for (int x = 0; x < windowWidth_; x++) {
            const int cellX = static_cast<int>(floorX);
            const int cellY = static_cast<int>(floorY);

            const int tx = static_cast<int>(floorAndCeilingTextures_[0]->getWidth() * (floorX - cellX)) & (floorAndCeilingTextures_[0]->getWidth() - 1);
            const int ty = static_cast<int>(floorAndCeilingTextures_[0]->getHeight() * (floorY - cellY)) & (floorAndCeilingTextures_[0]->getHeight() - 1);

            //const int floorTex = floorMap.at(tx, ty);

            floorX += floorStepX;
            floorY += floorStepY;

            const uint32_t floorColor = floorAndCeilingTextures_[0]->getPixel(tx, ty);
            framebuffer_[(offsetY_ + y) * windowWidth_ + (offsetX_ + x)] = floorColor;

            const int ceilingY = windowHeight_ - y - 1;
            const uint32_t ceilColor = floorAndCeilingTextures_[1]->getPixel(tx, ty);
            framebuffer_[(offsetY_ + ceilingY) * windowWidth_ + (offsetX_ + x)] = ceilColor;
        }
    }
}

void Raycaster::renderWalls(const Player& player, std::vector<double>& zBuffer) const {
    for (int x = 0; x < windowWidth_; x++) {
        // calculate ray position
        const double cameraX = 2 * x / static_cast<double>(windowWidth_) - 1;
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
        int side {}; // 0 = X side of the wall hit, 1 = Y side hit
        while (true) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= wallMap_.getWidth() || mapY < 0 || mapY >= wallMap_.getHeight()) {
                break;
            }

            if (wallMap_.at(mapX, mapY) > 0) {
                break;
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
        zBuffer[x] = perpWallDist;

        // to get perspective we calculate height of the wall from proportions
        const int lineHeight = static_cast<int>(windowHeight_ / perpWallDist);
        int drawStart = -lineHeight / 2 + windowHeight_ / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + windowHeight_ / 2;
        if (drawEnd >= windowHeight_) drawEnd = windowHeight_ - 1;

        int texIndex = wallMap_.at(mapX, mapY) - 1;
        const Texture* tex = wallTextures_[std::clamp(texIndex, 0, static_cast<int>(wallTextures_.size()-1))].get();

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

        const double step = 1.0 * tex->getHeight() / lineHeight;
        double texPos = (drawStart - windowHeight_ / 2.0 + lineHeight / 2.0) * step;

        for (int y = drawStart; y < drawEnd; y++) {
            const int texY = static_cast<int>(texPos) & (tex->getHeight() - 1);
            texPos += step;
            uint32_t color = tex->getPixel(texX, texY);
            // making 1-side darker. Shifting by 1 bit divides color by 2. Then we set first bit of each byte to 0
            if (side == 1) color = (color >> 1) & 0x7F7F7F7F;
            framebuffer_[(offsetY_ + y) * windowWidth_ + (offsetX_ + x)] = color;
        }
    }
}

void Raycaster::renderSprites(const Player& player, const std::vector<double>& zBuffer) const {
    // Sort sprites from the furthest to nearest
    std::vector<Entity*> sortedObjects;
    sortedObjects.reserve(entities_.size());

    for (const std::unique_ptr<Entity>& entity : entities_) {
        if (entity->visible_) {
            sortedObjects.push_back(entity.get());
        }
    }

    std::sort(sortedObjects.begin(), sortedObjects.end(), [&](const Entity* s1, const Entity* s2) {
        const double distS1 = (player.x - s1->position_.posX)*(player.x - s1->position_.posX) + (player.y - s1->position_.posY)*(player.y - s1->position_.posY);
        const double distS2 = (player.x - s2->position_.posX)*(player.x - s2->position_.posX) + (player.y - s2->position_.posY)*(player.y - s2->position_.posY);
        return distS1 > distS2;
    });

    // Projection
    for (const Entity* entity : sortedObjects) {
        // Translating sprite position to relative to player position
        const double objectX = entity->position_.posX - player.x;
        const double objectY = entity->position_.posY - player.y;

        // Transforming sprite position with inverse camera matrix
        // From Lodev's tutorial:
        // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
        // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
        // [ planeY   dirY ]                                          [ -planeY  planeX ]
        const double invDet = 1.0 / (player.planeX * player.dirY - player.dirX * player.planeY);

        const double transformX = invDet * (player.dirY * objectX - player.dirX * objectY);
        const double transformY = invDet * (-player.planeY * objectX + player.planeX * objectY);

        const int spriteScreenX = static_cast<int>(static_cast<int>(windowWidth_ / 2) * (1 + transformX / transformY));

        // Calculating sprite Height
        const int spriteHeight = std::abs(static_cast<int>(windowHeight_ / transformY)) / entity->sprite_->scale;

        // Calculating start and end of vertical stripe (from the middle of the screen)
        // vMoveScreen is shifting by Sprites shift parameter
        const int vMoveScreen = static_cast<int>(entity->sprite_->shift / transformY);
        int drawStartY = windowHeight_ / 2 - spriteHeight / 2 + vMoveScreen;
        int drawEndY = windowHeight_ / 2 + spriteHeight / 2 + vMoveScreen;

        const int clippedStartY = std::max(drawStartY, 0);
        const int clippedEndY = std::min(drawEndY, windowHeight_);

        // Calculating sprite width
        const int spriteWidth = std::abs(static_cast<int>(windowHeight_ / transformY)) / entity->sprite_->scale;
        const int drawStartX = spriteScreenX - spriteWidth / 2;
        const int drawEndX = spriteScreenX + spriteWidth / 2;

        const auto tex = entity->sprite_->model->texture.get();
        const int texWidth = tex->getWidth();
        const int texHeight = tex->getHeight();

        // Drawing in vertical stripes, so x first
        for (int stripe = drawStartX; stripe <= drawEndX; ++stripe) {
            const int localX = stripe - (spriteScreenX - spriteWidth / 2);
            const double texXf = (static_cast<double>(localX) / static_cast<double>(spriteWidth)) * texWidth;
            const int texX = std::clamp(static_cast<int>(texXf), 0, texWidth - 1);

            if (transformY > 0 && stripe > 0 && stripe < windowWidth_ && transformY < zBuffer[stripe]) {
                for (int y = clippedStartY; y < clippedEndY; ++y) {
                    const int localY = y - drawStartY;
                    const double texYf = (static_cast<double>(localY) / static_cast<double>(spriteHeight)) * texHeight;
                    const int texY = std::clamp(static_cast<int>(texYf), 0, texHeight - 1);

                    uint32_t color = tex->getPixel(texX, texY);
                    if ((color & 0x00FFFFFF) != 0) { // black = transparent
                        framebuffer_[(offsetY_ + y) * windowWidth_ + stripe] = color;
                    }
                }
            }
        }
    }
}

