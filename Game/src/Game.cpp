#include "Game/Game.h"

#include <chrono>

#include "Game/MainMenuState.h"

Game::Game()
    : engine(800, 600, "Game Title"),
    currentState(std::make_unique<MainMenuState>())
{}

void Game::run() {
    using clock = std::chrono::high_resolution_clock;
    auto current = clock::now();

    while (true) {
        auto now = clock::now();
        double dt = std::chrono::duration<double>(now - current).count();
        current = now;

        currentState->handleInput(engine);
        currentState->update(engine, dt);
        currentState->render(engine);

        if (currentState->isFinished()) {
            auto next = currentState->nextState();
            if (!next) break;
            currentState = std::move(next);
        }
    }

}

/*
void Game::loadLevel1() {

    engine.loadWallMap(Assets::path("/maps/world1/sample_map_walls.txt"));
    engine.loadFloorMap(Assets::path("/maps/world1/sample_map_floor.txt"));
    engine.loadCeilingMap(Assets::path("/maps/world1/sample_map_ceiling.txt"));

    engine.loadWallTexture(Assets::path("/textures/wall_stones.bmp") );
    engine.loadWallTexture( Assets::path("/textures/wall_brick.bmp") );

    engine.loadFloorCeilingTexture( Assets::path("/textures/floor_wood.bmp") );
    engine.loadFloorCeilingTexture( Assets::path("/textures/ceiling_teracote.bmp") );

    engine.loadSpriteTexture( Assets::path("/textures/wolftex/barrel.bmp") );
    engine.loadSpriteTexture( Assets::path("/textures/wolftex/pillar.bmp") );
    engine.loadSpriteTexture( Assets::path("/textures/wolftex/greenlight.bmp") );

    Player player {2.0, 2.0};
    engine.setPlayer(player);



    /*
    engine.addSprite(Sprite(3.5, 3.5, ));
    engine.addSprite(Sprite(4.5, 4.5, ...));
    engine.addSprite(Sprite(10.5, 10.5, ...));#1#
}
*/
