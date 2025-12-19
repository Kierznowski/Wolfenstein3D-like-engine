#pragma once

#include "Engine/Map.h"
#include "entity/Entity.h"

#include <memory>

class Player {
public:
    Player() = default;

    double planeX{0.0};
    double planeY{0.66};
    double dirX{-1.0};
    double dirY{0.0};
    double x{3.0};
    double y{3.0};
    double radius{0.3};
    const Map* wallMap {};
    const std::vector<std::unique_ptr<Entity>>* entities = nullptr;

    int health() const {
        return health_;
    }
    int ammo() const {
        return ammo_;
    }

    void increaseHealth(int boost);
    void decreaseHealth(int boost);

    void moveForward(double dt);
    void moveBackward(double dt);
    void moveLeft(double dt);
    void moveRight(double dt);
    void turnWithMouse(double rotationStep);
    void requestFire();
    bool consumeFireRequest();

private:
    double moveSpeed_{5.0}; // tiles per sec
    double rotationSpeed_{0.9}; // rads per sec
    int health_{80};
    int ammo_{50};
    bool wantsToFire_{false};
    const double MOUSE_SENSITIVITY_{0.01};

    bool collisionWithEntity(double testX, double testY);
};
