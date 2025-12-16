#pragma once

#include "commands/MoveBackwardCommand.h"
#include "commands/MoveForwardCommand.h"
#include "commands/TurnLeftCommand.h"
#include "commands/TurnRightCommand.h"
#include "Engine/GameState.h"
#include "Engine/Engine.h"

class GameplayState : public GameState {
public:
    explicit GameplayState(Engine& engine)
        : engine_(engine)
    {};

    void handleInput(const SDL_Event& event, CommandQueue& commandQueue) override;
    void handleRealtimeInput(CommandQueue& commandQueue) override;
    void render() override;

    int selected = 0;
private:
    MoveForwardCommand moveForwardCommand {};
    MoveBackwardCommand moveBackwardCommand {};
    TurnLeftCommand turnLeftCommand {};
    TurnRightCommand turnRightCommand {};
    Engine& engine_;
};