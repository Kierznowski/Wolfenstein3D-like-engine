#pragma once

#include "Engine/GameState.h"
#include "Engine/Engine.h"

class MainMenuState : public GameState {
public:
    MainMenuState() = default;

    void handleInput(SDL_Event& event) override;
    void handleInput(CommandQueue& commandQueue) override;
    void render(Engine& engine) override;

    int selected = 0;
};
