#pragma once
#include <Engine/Engine.h>
#include "GameState.h"

class Game {
public:
    Game();
    void run();

    //void loadLevel1();

private:
    Engine engine;
    std::unique_ptr<GameState> currentState;
};
