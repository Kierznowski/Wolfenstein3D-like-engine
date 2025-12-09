#pragma once
#include <Engine/Engine.h>

class Game {
public:
    Game();
    void run();

private:
    Engine* engine{};
    void loadAssets();
};
