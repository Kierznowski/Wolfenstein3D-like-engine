#pragma once
#include "Engine/entity/PickupEntity.h"

class HealthEntity : public PickupEntity {
public:
    explicit HealthEntity(
        int healthBoost=10.0,
        double posX=1.0,
        double posY=1.0,
        double radius=0.3,
        std::unique_ptr<Sprite> sprite=nullptr)
        : PickupEntity(posX, posY, radius, std::move(sprite)), healthBoost_(healthBoost) {
    }

    void interact(Player& player) override {
        if (!visible_) {
            return;
        }
        player.increaseHealth(healthBoost_);
        visible_=false;
    }

    void update(double dt) override {}
private:
    int healthBoost_;
};