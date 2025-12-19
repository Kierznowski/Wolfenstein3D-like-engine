#include "../include/Engine/Player.h"

#include <algorithm>
#include <cmath>

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

void Player::moveLeft(const double dt) {
    const double moveStep = moveSpeed_ * dt;

    // vector perpendicular to player dir, on the left
    const double strafeX = -dirY;
    const double strafeY =  dirX;

    const double nextX = x + strafeX * moveStep;
    const double nextY = y + strafeY * moveStep;

    if (wallMap->at(static_cast<int>(nextX), static_cast<int>(y)) == 0
        && !collisionWithEntity(nextX, y)) {
        x = nextX;
        }

    if (wallMap->at(static_cast<int>(x), static_cast<int>(nextY)) == 0
        && !collisionWithEntity(x, nextY)) {
        y = nextY;
        }

}

void Player::moveRight(const double dt) {
    const double moveStep = moveSpeed_ * dt;

    // vector perpendicular to player dir, on the right
    const double strafeX = dirY;
    const double strafeY = -dirX;

    const double nextX = x + strafeX * moveStep;
    const double nextY = y + strafeY * moveStep;

    if (wallMap->at(static_cast<int>(nextX), static_cast<int>(y)) == 0
        && !collisionWithEntity(nextX, y)) {
        x = nextX;
        }

    if (wallMap->at(static_cast<int>(x), static_cast<int>(nextY)) == 0
        && !collisionWithEntity(x, nextY)) {
        y = nextY;
        }

}

bool Player::collisionWithEntity(double testX, double testY) {
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

void Player::requestFire() {
    if (ammo_ > 0) {
        wantsToFire_ = true;
        ammo_ -= 1;
    }
}

bool Player::consumeFireRequest() {
    const bool r = wantsToFire_;
    wantsToFire_ = false;
    return r;
}

void Player::decreaseHealth(const int boost) {
    health_ -= boost;
    health_ = std::max(0, health_);
}

void Player::increaseHealth(const int boost) {
    health_ += boost;
    health_ = std::min(100, health_);
}

void Player::turnWithMouse(const double rotationStep) {
        const double oldDirX = dirX;
        dirX = dirX * std::cos(rotationStep) - dirY * std::sin(rotationStep);
        dirY = oldDirX * std::sin(rotationStep) + dirY * std::cos(rotationStep);

        const double oldPlaneX = planeX;
        planeX = planeX * std::cos(rotationStep) - planeY * std::sin(rotationStep);
        planeY = oldPlaneX * std::sin(rotationStep) + planeY * std::cos(rotationStep);
}