#pragma once
#include "Entity.h"

struct StaticEntity : Entity {
    StaticEntity(const double posX,
        const double posY,
        const double radius,
        std::unique_ptr<Sprite> sprite=nullptr)
    : Entity(posX, posY, radius, std::move(sprite), EntityType::STATIC)
    {
        solid_ = true;
        interactive_ = false;
        visible_ = true;
    };

    void interact(Player& player) override {};
    void update(double dt) override {};
};
