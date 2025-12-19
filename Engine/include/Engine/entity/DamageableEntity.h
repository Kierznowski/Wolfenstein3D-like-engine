#pragma once
#include "Entity.h"
#include "EntityType.h"
#include "Sprite/Sprite.h"

struct DamageableEntity : Entity {
    DamageableEntity(
        const double posX,
        const double posY,
        const double radius,
        std::unique_ptr<Sprite> sprite)
    : Entity(posX, posY, radius, std::move(sprite), EntityType::DAMAGEABLE)
    {
        solid_ = true;
        interactive_ = false;
        visible_ = true;
    }
    void onHit(Player& player, const int damage) {
        hp -= damage;
        if (hp <= 0) {
            alive = false;
            hp = 0;
        }
    }

    void update(double dt) override {
    }

    int hp {100};
};
