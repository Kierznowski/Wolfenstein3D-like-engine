#pragma once
#include "Command/CommandQueue.h"
#include <SDL2/SDL.h>

class Engine;

struct GameState {
    virtual ~GameState() = default;

    virtual void handleInput(const SDL_Event& event, CommandQueue& command_queue) = 0;
    virtual void handleRealtimeInput(CommandQueue& command_queue) = 0;
    virtual void render() = 0;
};