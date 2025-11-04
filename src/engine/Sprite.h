#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.h"

struct Sprite {
    double x;
    double y;
    const Texture* texture;

    Sprite(double posX, double posY, const Texture* texture)
        : x(posX), y(posY), texture(texture) {};
};


#endif // SPRITE_H