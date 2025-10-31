//
// Created by kierznowski on 27.10.2025.
//

#include "Game.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <cmath>

constexpr double FIXED_TICK_TIME = 1.0 / 60.0;

Game::Game(int width, int height)
    : renderer(width, height, "2.5D Engine") {

    const int tmpMap[mapWidth][mapHeight] = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
            {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        };

    std::memcpy(worldMap, tmpMap, sizeof(worldMap));

    wallTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wall_stones.bmp"));
    wallTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/wall_brick.bmp"));
    floorTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/floor_wood.bmp"));
    ceilingTextures.emplace_back(std::make_unique<Texture>("./../assets/textures/floor_stones.bmp"));

    player.x = 22.0;
    player.y = 12.0;
    player.dirX = -1.0;
    player.dirY = 0.0;
    player.planeX = 0.0;
    player.planeY = 0.66; // FOV 66 deg
}

void Game::run() {
    using clock = std::chrono::high_resolution_clock;
    auto currentTime = clock::now();
    double accumulator = 0.0;

    int frames = 0;
    double fpsTimer = 0.0;
    double fps = 0.0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        auto newTime = clock::now();
        std::chrono::duration<double> frameTime = newTime - currentTime;
        currentTime = newTime;
        double deltaTime = frameTime.count();
        if (deltaTime > 0.25) deltaTime = 0.25;
        accumulator += deltaTime;

        while (accumulator >= FIXED_TICK_TIME) {
            update(FIXED_TICK_TIME);
            accumulator -= FIXED_TICK_TIME;
        }

        renderer.clear();
        floorCeilingCasting(renderer.getWidth(), renderer.getHeight());
        ddaRaycasting();

        frames++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 1.0) {
            fps = frames / fpsTimer;
            std::cout << "FPS" << fps << std::endl;
            frames = 0;
            fpsTimer = 0.0;
        }
    }
}

void Game::update(double dt) {
    handleInput(dt);
}

void Game::floorCeilingCasting(int w, int h) {
    auto& framebuffer = renderer.getFramebuffer();
    double posX = player.x;
    double posY = player.y;
    const double dirX = player.dirX;
    const double dirY = player.dirY;
    const double planeX = player.planeX;
    const double planeY = player.planeY;

    for (int y = h / 2; y < h; y++) {
        const double rayDirX0 = dirX - planeX;
        const double rayDirY0 = dirY - planeY;
        const double rayDirX1 = dirX + planeX;
        const double rayDirY1 = dirY + planeY;

        const int p = y - h / 2;
        const double posZ = h / 2.0; // middle of the screen
        const double rowDistance = posZ / p;

        const double floorStepX = rowDistance * (rayDirX1 - rayDirX0) / w;
        const double floorStepY = rowDistance * (rayDirY1 - rayDirY0) / w;

        double floorX = posX + rowDistance * rayDirX0;
        double floorY = posY + rowDistance * rayDirY0;

        for (int x = 0; x < w; x++) {
            const int cellX = static_cast<int>(floorX);
            const int cellY = static_cast<int>(floorY);

            const int tx = static_cast<int>(floorTextures[0]->getWidth() * (floorX - cellX)) & (floorTextures[0]->getWidth() - 1);
            const int ty = static_cast<int>(floorTextures[0]->getHeight() * (floorY - cellY)) & (floorTextures[0]->getHeight() - 1);

            floorX += floorStepX;
            floorY += floorStepY;

            const uint32_t colorFloor = floorTextures[0]->getPixel(tx, ty);
            framebuffer[y * w + x] = colorFloor;

            const int ceilingY = h - y - 1;
            const uint32_t colorCeil = ceilingTextures[0]->getPixel(tx, ty);
            framebuffer[ceilingY * w + x] = colorCeil;
        }
    }


}

void Game::ddaRaycasting() {
    const int w {renderer.getWidth()};
    const int h {renderer.getHeight()};

    auto& framebuffer = renderer.getFramebuffer();

    for (int x = 0; x < w; x++) {
        // calculate ray position
        const double cameraX = 2 * x / static_cast<double>(w) - 1;
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
            if (worldMap[mapX][mapY] > 0) {
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
        const int lineHeight = static_cast<int>(h / perpWallDist);
        int drawStart = -lineHeight / 2 + h / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + h / 2;
        if (drawEnd >= h) drawEnd = h - 1;

        int texIndex = worldMap[mapX][mapY] - 1;
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
        double texPos = (drawStart - h / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++) {
            int texY = static_cast<int>(texPos) & (tex->getHeight() - 1);
            texPos += step;
            uint32_t color = tex->getPixel(texX, texY);
            // making 1-side darker. Shifting by 1 bit divides color by 2. Then we set first bit of each byte to 0
            if (side == 1) color = (color >> 1) & 0x7F7F7F7F;
            framebuffer[y * w + x] = color;
        }
    }

    renderer.presentFrame(framebuffer);
}

void Game::handleInput(double dt) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    const double moveStep = player.moveSpeed * dt;
    const double rotationStep = player.rotationSpeed * dt;

    if (state[SDL_SCANCODE_W]) {
        double nextX = player.x + player.dirX * moveStep;
        double nextY = player.y + player.dirY * moveStep;

        if (worldMap[static_cast<int>(nextX)][static_cast<int>(player.y)] == 0) {
            player.x = nextX;
        }
        if (worldMap[static_cast<int>(player.x)][static_cast<int>(nextY)] == 0) {
            player.y = nextY;
        }
    }
    if (state[SDL_SCANCODE_S]) {
        double nextX = player.x - player.dirX * moveStep;
        double nextY = player.y - player.dirY * moveStep;

        if (worldMap[static_cast<int>(nextX)][static_cast<int>(player.y)] == 0) {
            player.x = nextX;
        }
        if (worldMap[static_cast<int>(player.x)][static_cast<int>(nextY)] == 0) {
            player.y = nextY;
        }
    }

    if (state[SDL_SCANCODE_A]) {
        const double oldDirX = player.dirX;
        player.dirX = player.dirX * std::cos(rotationStep) - player.dirY * std::sin(rotationStep);
        player.dirY = oldDirX * std::sin(rotationStep) + player.dirY * std::cos(rotationStep);

        const double oldPlaneX = player.planeX;
        player.planeX = player.planeX * std::cos(rotationStep) - player.planeY * std::sin(rotationStep);
        player.planeY = oldPlaneX * std::sin(rotationStep) + player.planeY * std::cos(rotationStep);
    }

    if (state[SDL_SCANCODE_D]) {
        const double oldDirX = player.dirX;
        player.dirX = player.dirX * std::cos(-rotationStep) - player.dirY * std::sin(-rotationStep);
        player.dirY = oldDirX * std::sin(-rotationStep) + player.dirY * std::cos(-rotationStep);

        const double oldPlaneX = player.planeX;
        player.planeX = player.planeX * std::cos(-rotationStep) - player.planeY * std::sin(-rotationStep);
        player.planeY = oldPlaneX * std::sin(-rotationStep) + player.planeY * std::cos(-rotationStep);
    }
}