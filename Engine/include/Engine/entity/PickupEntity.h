#pragma once
#include <memory>

#include "Entity.h"

struct PickupEntity : Entity {
    PickupEntity(const double posX,
        const double posY,
        const double radius,
        std::unique_ptr<Sprite> sprite=nullptr)
    : Entity(posX, posY, radius, std::move(sprite), EntityType::PICKUP)
    {
        solid_ = false;
        interactive_ = true;
        visible_ = true;
    };

    void interact(Player& player) override {
        visible_ = false;
    };

    void update(double dt) override {}
};
