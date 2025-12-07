#include "Game/GameplayState.h"

#include "Game/commands/MoveForwardCommand.h"
#include "Game/commands/MoveBackwardCommand.h"
#include "Game/commands/TurnLeftCommand.h"
#include "Game/commands/TurnRightCommand.h"

void GameplayState::handleInput(CommandQueue& commandQueue) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_W]) {
        commandQueue.push(std::make_unique<MoveForwardCommand>());
    }
    if (state[SDL_SCANCODE_S]) {
        commandQueue.push(std::make_unique<MoveBackwardCommand>());
    }
    if (state[SDL_SCANCODE_A]) {
        commandQueue.push(std::make_unique<TurnLeftCommand>());
    }
    if (state[SDL_SCANCODE_D]) {
        commandQueue.push(std::make_unique<TurnRightCommand>());
    }
}

void GameplayState::handleInput(SDL_Event& event) {
}

void GameplayState::render(Engine& engine) {
    engine.render();
}

void checkTransitions(Engine& engine) {
}
