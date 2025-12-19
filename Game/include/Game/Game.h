#pragma once
#include <Engine/Engine.h>

class Game {
public:
    Game();
    void run();

private:
    std::unique_ptr<Player> player_;

    void loadAssets(Engine& engine) const;
    void createHud(Engine& engine);
};
