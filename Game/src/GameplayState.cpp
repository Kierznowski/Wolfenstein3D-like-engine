#include "Game/GameplayState.h"

#include "Game/commands/MouseTurnCommand.h"

void GameplayState::handleInput(const SDL_Event& event, CommandQueue& commandQueue) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        commandQueue.push(std::make_unique<FireCommand>());
    }
    if (event.type == SDL_MOUSEMOTION) {
        commandQueue.push(std::make_unique<MouseTurnCommand>(static_cast<double>(-event.motion.xrel)));
    }
}

void GameplayState::handleRealtimeInput(CommandQueue& commandQueue) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_W])
        commandQueue.push(std::make_unique<MoveForwardCommand>());

    if (state[SDL_SCANCODE_S])
        commandQueue.push(std::make_unique<MoveBackwardCommand>());

    if (state[SDL_SCANCODE_A])
        commandQueue.push(std::make_unique<TurnLeftCommand>());

    if (state[SDL_SCANCODE_D])
        commandQueue.push(std::make_unique<TurnRightCommand>());
}

void GameplayState::render() {
    engine_.render();
}