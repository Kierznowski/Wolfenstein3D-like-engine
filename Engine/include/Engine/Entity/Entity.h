#pragma once
#include "Engine/Utils/Vector2.h"
#include "Sprite/Sprite.h"

class Player;

class Entity {
public:
    virtual ~Entity() = default;

    virtual void update(double dt) = 0;
    virtual void interact(Player& player) = 0;

    Vector2 position_ {1.0, 1.0};
    double radius_ {0.2};
    bool solid_ {false};
    bool interactive_ {false};
    bool visible_ {true};
    bool hp_ {false};
    std::unique_ptr<Sprite> sprite_ {nullptr};
};
