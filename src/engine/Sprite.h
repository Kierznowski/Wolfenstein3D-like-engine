#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.h"

struct Sprite {
    double x;
    double y;
    const Texture* texture;
    const double scale;
    const double shift;

    Sprite(double posX, double posY, const Texture* texture, double scale, double shift)
        : x{posX}, y{posY}, texture{texture}, scale{scale}, shift{shift} {}
};

#endif // SPRITE_H