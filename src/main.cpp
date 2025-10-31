#include <iostream>

#include "engine/Game.h"


constexpr int WINDOW_W = 720;
constexpr int WINDOW_H = 640;

int main() {

    try {
        Game game(WINDOW_W, WINDOW_H);
        game.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
