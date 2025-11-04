#ifndef SPRITE_H
#define SPRITE_H
#include "Texture.h"

struct Sprite {
    double posX;
    double posY;
    const Texture* texture;

    Sprite(double posX, double posY, Texture* texture)
        : posX(posX), posY(posY), texture(texture) {};
};


#endif // SPRITE_H