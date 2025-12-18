#pragma once
#include "Engine/entity/DamageableEntity.h"

class DamagableBarrelEntity : public DamageableEntity {
public:
    explicit DamagableBarrelEntity(double x = 1.0,
        double y = 1.0,
        double radius = 0.1,
        std::unique_ptr<Sprite> sprite = nullptr)
    : DamageableEntity(x, y, radius, std::move(sprite))
    {}

    void update(double dt) override {}
    void interact(Player& player) override {}

};