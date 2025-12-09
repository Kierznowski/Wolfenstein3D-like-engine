#pragma once

#include "Engine/GameState.h"
#include "Engine/Engine.h"

class GameplayState : public GameState {
public:
    GameplayState() = default;

    void handleInput(SDL_Event& event) override;
    void handleInput(CommandQueue& commandQueue) override;
    void render(Engine& engine) override;

    int selected = 0;
};