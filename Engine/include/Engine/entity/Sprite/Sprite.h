#pragma once
#include "SpriteModel.h"

struct Sprite {
    Sprite(const std::shared_ptr<SpriteModel>& spriteModel,
        const double scale,
        const double shift)
            : scale(scale), shift(shift), model(spriteModel)
    {}

    Sprite(const std::shared_ptr<SpriteModel>& spriteModel)
        : model(spriteModel)
    {}

    const double scale {1.0};
    const double shift {0.0};
    std::shared_ptr<SpriteModel> model;
};