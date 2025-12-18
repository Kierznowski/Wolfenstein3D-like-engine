#pragma once
#include <memory>

#include "Entity.h"
#include "EntityType.h"

struct InteractiveEntity : Entity {

    InteractiveEntity(
        const double posX,
        const double posY,
        const double radius,
        std::unique_ptr<Sprite> sprite)
    : Entity(posX, posY, radius, std::move(sprite), EntityType::INTERACTIVE)
    {
        solid_ = false;
        interactive_ = true;
        visible_ = true;
    };

    void update(double dt) override;
    void interact(Player& player) override;
};
