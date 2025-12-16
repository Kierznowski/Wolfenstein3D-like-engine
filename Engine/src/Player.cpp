#include "../include/Engine/Player.h"

#include <cmath>
#include <iostream>

void Player::update(const double dt) {

}

void Player::moveForward(const double dt) {
    const double moveStep = moveSpeed_ * dt;
    const double nextX = x + dirX * moveStep;
    const double nextY = y + dirY * moveStep;

    if (wallMap->at(static_cast<int>(nextX), static_cast<int>(y)) == 0
        && !collisionWithEntity(nextX, y)) {
        x = nextX;
    }
    if (wallMap->at(static_cast<int>(x), static_cast<int>(nextY)) == 0
        && !collisionWithEntity(x, nextY)) {
        y = nextY;
    }
}

void Player::moveBackward(const double dt) {
    const double moveStep = moveSpeed_ * dt;

    const double nextX = x - dirX * moveStep;
    const double nextY = y - dirY * moveStep;

    if (wallMap->at(static_cast<int>(nextX), static_cast<int>(y)) == 0
        && !collisionWithEntity(nextX, y)) {
        x = nextX;
    }
    if (wallMap->at(static_cast<int>(x), static_cast<int>(nextY)) == 0
        && !collisionWithEntity(x, nextY)) {
        y = nextY;
    }
}

void Player::turnLeft(const double dt) {
    const double rotationStep = rotationSpeed_ * dt;

    const double oldDirX = dirX;
    dirX = dirX * std::cos(rotationStep) - dirY * std::sin(rotationStep);
    dirY = oldDirX * std::sin(rotationStep) + dirY * std::cos(rotationStep);

    const double oldPlaneX = planeX;
    planeX = planeX * std::cos(rotationStep) - planeY * std::sin(rotationStep);
    planeY = oldPlaneX * std::sin(rotationStep) + planeY * std::cos(rotationStep);

}

void Player::turnRight(const double dt) {
    const double rotationStep = rotationSpeed_ * dt;

    const double oldDirX = dirX;
    dirX = dirX * std::cos(-rotationStep) - dirY * std::sin(-rotationStep);
    dirY = oldDirX * std::sin(-rotationStep) + dirY * std::cos(-rotationStep);

    const double oldPlaneX = planeX;
    planeX = planeX * std::cos(-rotationStep) - planeY * std::sin(-rotationStep);
    planeY = oldPlaneX * std::sin(-rotationStep) + planeY * std::cos(-rotationStep);
}

bool Player::collisionWithEntity(double testX, double testY) const {
    if (!entities) return false;

    for (const auto& e : *entities) {

        const double dx = testX - e->position_.posX;
        const double dy = testY - e->position_.posY;
        const double dist2 = dx * dx + dy * dy;
        const double minDist = radius + e->radius_;

        if (dist2 < minDist * minDist) {
            if (!e->solid_) {
                e->interact(*this);
                return false;
            }
            return true;
        }
    }
    return false;
}
