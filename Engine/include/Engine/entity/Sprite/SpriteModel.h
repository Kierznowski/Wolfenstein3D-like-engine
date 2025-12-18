#pragma once
#include <iostream>
#include <memory>

#include "../../Texture.h"

struct SpriteModel {
    explicit SpriteModel(const std::shared_ptr<Texture>& texture)
        : texture(texture) {
        std::cout << "Model created" << std::endl;
    };
    const std::shared_ptr<Texture> texture;
};
