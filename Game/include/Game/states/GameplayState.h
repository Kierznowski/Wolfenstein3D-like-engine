#pragma once

#include "../commands/FireCommand.h"
#include "../commands/MoveBackwardCommand.h"
#include "../commands/MoveForwardCommand.h"
#include "../commands/TurnLeftCommand.h"
#include "../commands/TurnRightCommand.h"
#include "Engine/GameState.h"
#include "Engine/Engine.h"

class GameplayState : public GameState {
public:
    explicit GameplayState(Engine& engine)
        : engine_(engine) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    };

    void handleInput(const SDL_Event& event, CommandQueue& commandQueue) override;
    void handleRealtimeInput(CommandQueue& commandQueue) override;
    void render() override;

    int selected = 0;
private:
    MoveForwardCommand moveForwardCommand {};
    MoveBackwardCommand moveBackwardCommand {};
    TurnLeftCommand turnLeftCommand {};
    TurnRightCommand turnRightCommand {};
    FireCommand fireCommand {};
    Engine& engine_;
};