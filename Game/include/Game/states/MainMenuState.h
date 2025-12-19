#pragma once

#include "Engine/GameState.h"
#include "Engine/Engine.h"

class MainMenuState : public GameState {
public:
    explicit MainMenuState(Engine& engine)
        : engine(engine)
    {};

    void handleInput(const SDL_Event& event, CommandQueue& commandQueue) override;
    void handleRealtimeInput(CommandQueue& commandQueue) override;
    void render() override;

    int selected = 0;

private:
    Engine& engine;
};
