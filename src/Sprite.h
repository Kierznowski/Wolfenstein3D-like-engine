#pragma once

struct Sprite {
    double x; // world coordinates
    double y;
    int textureIndex;

    Sprite(double x, double y, int textureIndex)
        : x(x), y(y), textureIndex(textureIndex) {}
};

