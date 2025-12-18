#include "Game/Game.h"

#include "Game/MainMenuState.h"

#include <../../Engine/include/Engine/utils/Assets.h>

#include "Engine/entity/DamageableEntity.h"
#include "Engine/entity/PickupEntity.h"
#include "Engine/entity/StaticEntity.h"
#include "Game/entities/DamagableBarrelEntity.h"
#include "Game/entities/HealthEntity.h"

Game::Game() = default;

void Game::run() {
    Engine engine{800, 640};

    player = std::make_unique<Player>();
    engine.setPlayer(player.get());

    createHud(engine);
    loadAssets(engine);
    engine.setState(std::make_unique<MainMenuState>(engine));
    engine.run();
}

void Game::createHud(Engine& engine) const {
    auto font = std::make_unique<BitmapFont>(Assets::path("/fonts/bitmapFont1.bmp"), 16, 17,
        "0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    auto healthIcon = std::make_unique<Texture>(Assets::path("/textures/hp.bmp"));
    engine.loadHud(std::make_unique<HUD>(engine.getWindowWidth(), engine.getWindowHeight(), std::move(font), std::move(healthIcon), engine.getWallMap()));
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
    const auto barrelSprite = Sprite(barrelModel, 1.0, 0.0);

    const auto healthModel = engine.loadSpriteModel("health", Assets::path("/textures/health.bmp"));
    const auto healthSprite = Sprite(healthModel, 4.0, 250.0);


    engine.loadEntity(
        std::make_unique<HealthEntity>(15, 12.0, 12.0, 0.1, std::make_unique<Sprite>(healthSprite)));

    engine.loadEntity(
        std::make_unique<DamagableBarrelEntity>(20.0, 20.0, 0.3, std::make_unique<Sprite>(barrelSprite)));


    engine.loadEntity(
        std::make_unique<StaticEntity>(15.0, 15.0, 0.1, std::make_unique<Sprite>(barrelSprite)));


    engine.loadEntity(
        std::make_unique<PickupEntity>(10.0, 10.0, 0.3, std::make_unique<Sprite>(barrelSprite)));
}