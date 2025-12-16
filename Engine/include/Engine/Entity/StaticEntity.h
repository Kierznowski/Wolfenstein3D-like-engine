#pragma once
#include "Entity.h"

struct StaticEntity : Entity {
    StaticEntity(const double posX,
        const double posY,
        const double radius,
        const Sprite* sprite)
    {
        position_ = {posX, posY};
        radius_ = radius;
        sprite_ = std::make_unique<Sprite>(*sprite);
        solid_ = true;
        interactive_ = false;
        visible_ = true;
        hp_ = false;
    };

    void update(double dt) override {};

    void interact(const Player& player) override {};
};
