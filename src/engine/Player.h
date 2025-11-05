#ifndef PLAYER_H
#define PLAYER_H

struct Player {
    double x{22.0};
    double y{12.0};
    double dirX{-1.0};
    double dirY{0.0};
    double planeX{0.0};
    double planeY{0.66};

    double moveSpeed{3.0}; // tiles per sec
    double rotationSpeed{2.0}; // rads per sec
};

#endif // PLAYER_H