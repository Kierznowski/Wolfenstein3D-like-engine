#pragma once
#include <memory>

#include "Entity.h"

struct InteractiveEntity : Entity {

    InteractiveEntity(const double posX,
        const double posY,
        const double radius,
        const Sprite* sprite)
    {
        position_ = {posX, posY};
        radius_ = radius;
        sprite_ = std::make_unique<Sprite>(*sprite);
        solid_ = false;
        interactive_ = true;
        visible_ = true;
        hp_ = false;
    };

    void update(double dt) override;

    void interact(Player& player) override;
};