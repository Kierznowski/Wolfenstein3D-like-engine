#pragma once
#include "EntityType.h"
#include "Engine/utils/Vector2.h"
#include "Sprite/Sprite.h"

class Player;

struct Entity {
    Entity(
        double x,
        double y,
        double radius,
        std::unique_ptr<Sprite> sprite,
        EntityType type)
        : position_(x, y), radius_(radius), sprite_(std::move(sprite)), type_(type)
    {};

    virtual ~Entity() = default;

    virtual void update(double dt) = 0;
    virtual void interact(Player& player) = 0;

    Vector2 position_ {1.0, 1.0};
    double radius_ {0.2};
    bool solid_ {false};
    bool interactive_ {false};
    bool visible_ {true};
    std::unique_ptr<Sprite> sprite_;
    EntityType type_;
    bool alive {true};
};
