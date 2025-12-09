#pragma once
#include "SpriteModel.h"

struct Sprite {
    Sprite(double posX, double posY, const std::shared_ptr<SpriteModel>& spriteModel, double scale, double shift)
        : x(posX), y(posY), model(spriteModel), scale(scale), shift(shift)
    {}

    Sprite(double posX, double posY, const std::shared_ptr<SpriteModel>& spriteModel)
        : x(posX), y(posY), model(spriteModel)
    {}

    double x;
    double y;

    const double scale {1.0};
    const double shift {0.0};
    const std::shared_ptr<SpriteModel> model;
};