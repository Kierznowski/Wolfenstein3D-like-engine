#pragma once
#include "Entity.h"
#include "Sprite/Sprite.h"

struct DamageableEntity : Entity {
    DamageableEntity(
        const double posX,
        const double posY,
        const double radius,
        const Sprite* sprite)
    {
        position_ = { posX, posY };
        radius_ = radius;
        sprite_ = sprite;
        solid_ = true;
        interactive_ = false;
        visible_ = true;
        hp_ = true;
    }

};
