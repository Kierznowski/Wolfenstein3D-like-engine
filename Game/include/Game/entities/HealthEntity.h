#pragma once
#include "Engine/Entity/PickupEntity.h"

class HealthEntity : public PickupEntity {
public:
    explicit HealthEntity(
        const int healthBoost=10.0,
        const double posX=1.0,
        const double posY=1.0,
        const double radius=0.3,
        const Sprite* sprite=nullptr)
        : PickupEntity(posX, posY, radius, sprite), healthBoost_(healthBoost)
    {}

    void interact(Player& player) override {
        if (!visible_) {
            return;
        }
        player.addHealth(healthBoost_);
        visible_=false;
    }
private:
    int healthBoost_;
};