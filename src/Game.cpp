#include "Game.h"
#include <iostream>
#include <memory>

Game::Game() {
    init();
}

Game::~Game() {
    cleanup();
}

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Wolfenstein style engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window not initialized! SDL Error: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer) {
        std::cerr << "Renderer could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    renderer = std::make_unique<Renderer>(sdlRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    raycaster = std::make_unique<Raycaster>(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    raycaster->setMap(map);
    raycaster->setPlayer(player);

    running = true;
}

void Game::run() {
    while (running) {
        handleEvents();
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
    }

    const Uint8* keyStates = SDL_GetKeyboardState(NULL);

    double moveStep = player.moveSpeed;
    double rotationStep = player.rotationSpeed;

    if (keyStates[SDL_SCANCODE_W]) {
        double nextX = player.posX + player.dirX * moveStep;
        double nextY = player.posY + player.dirY * moveStep;
        if (!map.isWall(static_cast<int>(nextX), static_cast<int>(player.posY))) {
            player.posX = nextX;
        }
        if (!map.isWall(static_cast<int>(player.posX), static_cast<int>(nextY))) {
            player.posY = nextY;
        }
    }
    if (keyStates[SDL_SCANCODE_S]) {
        double nextX = player.posX - player.dirX * moveStep;
        double nextY = player.posY - player.dirY * moveStep;
        if (!map.isWall(static_cast<int>(nextX), static_cast<int>(player.posY))) {
            player.posX = nextX;
        }
        if (!map.isWall(static_cast<int>(player.posX), static_cast<int>(nextY))) {
            player.posY = nextY;
        }
    }
    if (keyStates[SDL_SCANCODE_A]) {
        double oldDirX = player.dirX;
        player.dirX = player.dirX * std::cos(rotationStep) - player.dirY * std::sin(rotationStep);
        player.dirY = oldDirX * std::sin(rotationStep) + player.dirY * std::cos(rotationStep);

        double oldPlaneX = player.planeX;
        player.planeX = player.planeX * std::cos(rotationStep) - player.planeY * std::sin(rotationStep);
        player.planeY = oldPlaneX * std::sin(rotationStep) + player.planeY * std::cos(rotationStep);
    }
    if (keyStates[SDL_SCANCODE_D]) {
        double oldDirX = player.dirX;
        player.dirX = player.dirX * std::cos(-rotationStep) - player.dirY * std::sin(-rotationStep);
        player.dirY = oldDirX * std::sin(-rotationStep) + player.dirY * std::cos(-rotationStep);

        double oldPlaneX = player.planeX;
        player.planeX = player.planeX * std::cos(-rotationStep) - player.planeY * std::sin(-rotationStep);
        player.planeY = oldPlaneX * std::sin(-rotationStep) + player.planeY * std::cos(-rotationStep);
    }
    if (keyStates[SDL_SCANCODE_E]) {
        double strafeX = player.dirY;
        double strafeY = -player.dirX;
        double nextX = player.posX + strafeX * moveStep;
        double nextY = player.posY + strafeY * moveStep;
        if (!map.isWall(static_cast<int>(nextX), static_cast<int>(player.posY))) {
            player.posX = nextX;
        }
        if (!map.isWall(static_cast<int>(player.posX), static_cast<int>(nextY))) {
            player.posY = nextY;
        }
    }
    if (keyStates[SDL_SCANCODE_Q]) {
        double strafeX = -player.dirY;
        double strafeY = player.dirX;
        double nextX = player.posX + strafeX * moveStep;
        double nextY = player.posY + strafeY * moveStep;
        if (!map.isWall(static_cast<int>(nextX), static_cast<int>(player.posY))) {
            player.posX = nextX;
        }
        if (!map.isWall(static_cast<int>(player.posX), static_cast<int>(nextY))) {
            player.posY = nextY;
        }
    }
    raycaster->setPlayer(player);

}

void Game::render() {
    renderer->clear(0xFF000000);
    raycaster->renderFrame();
    renderer->present();
}

void Game::cleanup() {
    if (sdlRenderer) SDL_DestroyRenderer(sdlRenderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

