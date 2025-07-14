#pragma once
#include <cmath>

struct Player {
    double posX = 22.0;
    double posY = 12.0;

    double dirX = -1.0;
    double dirY = 0.0;

    double planeX = 0.0;
    double planeY = 0.66;

    double moveSpeed = 0.05;
    double rotationSpeed = 0.03;
};

