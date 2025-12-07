#pragma once

#include "Engine/Map.h"

struct Player {
    double x{3.0};
    double y{3.0};
    double dirX{-1.0};
    double dirY{0.0};
    double planeX{0.0};
    double planeY{0.66};

    double moveSpeed{10.0}; // tiles per sec
    double rotationSpeed{5.0}; // rads per sec

    Map* wallMap;

    void moveForward(double dt);
    void moveBackward(double dt);
    void turnLeft(double dt);
    void turnRight(double dt);
};
