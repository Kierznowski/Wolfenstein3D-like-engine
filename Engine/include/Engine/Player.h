#pragma once

#include <memory>

#include "Engine/Map.h"
#include "Entity/Entity.h"

class Player {
public:
    Player()
    = default;

    double planeX{0.0};
    double planeY{0.66};
    double dirX{-1.0};
    double dirY{0.0};
    double x{3.0};
    double y{3.0};
    double radius{0.3};
    const Map* wallMap {};
    const std::vector<std::unique_ptr<Entity>>* entities = nullptr;
    int getHealth() const {
        return health_;
    }
    void addHealth(const int boost) {
        health_ += boost;
        health_ = std::min<int>(health_, 100);
    }
    void removeHealth(const int boost) {
        health_ -= boost;
    }

    void moveForward(double dt);
    void moveBackward(double dt);
    void turnLeft(double dt);
    void turnRight(double dt);
    void update(double dt);
private:
    double moveSpeed_{8.0}; // tiles per sec
    double rotationSpeed_{5.0}; // rads per sec
    int health_{100};

    bool collisionWithEntity(double testX, double testY) const;
};
