#include "Game/Game.h"

#include "Game/MainMenuState.h"

#include <Engine/Assets.h>

#include "Engine/Entity/PickupEntity.h"
#include "Engine/Entity/StaticEntity.h"
#include "Game/entities/HealthEntity.h"

Game::Game() = default;

void Game::run() {
    Engine engine{};

    player = std::make_unique<Player>();
    engine.setPlayer(player.get());

    loadAssets(engine);
    engine.setState(std::make_unique<MainMenuState>(engine));
    engine.run();
}

void Game::loadAssets(Engine& engine) const {
    engine.loadWallMap(Assets::path("/maps/world1/sample_map_walls.txt"));
    engine.loadFloorMap(Assets::path("/maps/world1/sample_map_floor.txt"));
    engine.loadCeilingMap(Assets::path("/maps/world1/sample_map_ceiling.txt"));

    engine.loadWallTexture(Assets::path("/textures/wall_stones.bmp") );
    engine.loadWallTexture( Assets::path("/textures/wall_brick.bmp") );

    engine.loadFloorCeilingTexture( Assets::path("/textures/floor_wood.bmp") );
    engine.loadFloorCeilingTexture( Assets::path("/textures/ceiling_teracote.bmp") );

    const auto barrelModel = engine.loadSpriteModel("barrel", Assets::path("/textures/wolftex/barrel.bmp"));
    const auto barrelSprite = std::make_unique<Sprite>(barrelModel, 1.0, 0.0);

    const auto healthModel = engine.loadSpriteModel("health", Assets::path("/textures/health.bmp"));
    const auto healthSprite = std::make_unique<Sprite>(healthModel, 4.0, 250);


    engine.loadEntity(
        std::make_unique<HealthEntity>(15, 12.0, 12.0, 0.1, healthSprite.get()));

    engine.loadEntity(
        std::make_unique<StaticEntity>(20.0, 20.0, 0.1, barrelSprite.get()));


    engine.loadEntity(
        std::make_unique<StaticEntity>(15.0, 15.0, 0.1, barrelSprite.get()));

    engine.loadEntity(
        std::make_unique<PickupEntity>(10.0, 10.0, 0.3, barrelSprite.get()));
}
