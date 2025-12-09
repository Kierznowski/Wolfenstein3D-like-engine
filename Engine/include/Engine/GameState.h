#pragma once
#include <memory>
#include "Engine/CommandQueue.h"
#include <SDL2/SDL.h>

#include "Renderer.h"

class Engine;

struct GameState {
    GameState() = default;
    virtual void handleInput(SDL_Event& event) = 0;
    virtual void handleInput(CommandQueue& commandQueue) = 0;
    virtual void render(Engine& engine) = 0;

    bool isFinished = false;
    std::unique_ptr<GameState> nextState;
};