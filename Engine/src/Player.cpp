#include "Engine/Player.h"

#include <cmath>

void Player::moveForward(double dt) {
    const double moveStep = moveSpeed * dt;
    const double nextX = x + dirX * moveStep;
    const double nextY = y + dirY * moveStep;

    if (wallMap->at(static_cast<int>(nextX), static_cast<int>(y)) == 0) {
        x = nextX;
    }
    if (wallMap->at(static_cast<int>(x), static_cast<int>(nextY)) == 0) {
        y = nextY;
    }
}

void Player::moveBackward(double dt) {
    const double moveStep = moveSpeed * dt;

    const double nextX = x - dirX * moveStep;
    const double nextY = y - dirY * moveStep;

    if (wallMap->at(static_cast<int>(nextX), static_cast<int>(y)) == 0) {
        x = nextX;
    }
    if (wallMap->at(static_cast<int>(x), static_cast<int>(nextY)) == 0) {
        y = nextY;
    }
}

void Player::turnLeft(double dt) {
    const double rotationStep = rotationSpeed * dt;

    const double oldDirX = dirX;
    dirX = dirX * std::cos(rotationStep) - dirY * std::sin(rotationStep);
    dirY = oldDirX * std::sin(rotationStep) + dirY * std::cos(rotationStep);

    const double oldPlaneX = planeX;
    planeX = planeX * std::cos(rotationStep) - planeY * std::sin(rotationStep);
    planeY = oldPlaneX * std::sin(rotationStep) + planeY * std::cos(rotationStep);

}

void Player::turnRight(double dt) {
    const double rotationStep = rotationSpeed * dt;

    const double oldDirX = dirX;
    dirX = dirX * std::cos(-rotationStep) - dirY * std::sin(-rotationStep);
    dirY = oldDirX * std::sin(-rotationStep) + dirY * std::cos(-rotationStep);

    const double oldPlaneX = planeX;
    planeX = planeX * std::cos(-rotationStep) - planeY * std::sin(-rotationStep);
    planeY = oldPlaneX * std::sin(-rotationStep) + planeY * std::cos(-rotationStep);

}