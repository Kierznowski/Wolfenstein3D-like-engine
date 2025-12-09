#include "Game/Game.h"

#include "Game/MainMenuState.h"

#include <Engine/Assets.h>
#include <iostream>

#include "Game/GameplayState.h"

Game::Game() = default;

void Game::run() {
    engine = new Engine(800, 600, "Game Title");

    loadAssets();

    engine->setState(std::make_unique<MainMenuState>());
    engine->run();
}


void Game::loadAssets() {

    engine->loadWallMap(Assets::path("/maps/world1/sample_map_walls.txt"));
    engine->loadFloorMap(Assets::path("/maps/world1/sample_map_floor.txt"));
    engine->loadCeilingMap(Assets::path("/maps/world1/sample_map_ceiling.txt"));

    engine->loadWallTexture(Assets::path("/textures/wall_stones.bmp") );
    engine->loadWallTexture( Assets::path("/textures/wall_brick.bmp") );

    engine->loadFloorCeilingTexture( Assets::path("/textures/floor_wood.bmp") );
    engine->loadFloorCeilingTexture( Assets::path("/textures/ceiling_teracote.bmp") );

    /*
    engine->loadSpriteTexture(Assets::path("/textures/wolftex/barrel.bmp") );
    engine->loadSpriteTexture(Assets::path("/textures/wolftex/pillar.bmp") );
    engine->loadSpriteTexture(Assets::path("/textures/wolftex/greenlight.bmp") );
    */

    auto barrelModel = engine->loadSpriteModel("barrel", Assets::path("/textures/wolftex/barrel.bmp"));
    engine->loadSprite(Sprite(3.5, 3.5, barrelModel));

}
