#include "Game/GameplayState.h"

void GameplayState::handleInput(const SDL_Event& event, CommandQueue& commandQueue) {
}

void GameplayState::handleRealtimeInput(CommandQueue& commandQueue) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_W])
        commandQueue.push(&moveForwardCommand);

    if (state[SDL_SCANCODE_S])
        commandQueue.push(&moveBackwardCommand);

    if (state[SDL_SCANCODE_A])
        commandQueue.push(&turnLeftCommand);

    if (state[SDL_SCANCODE_D])
        commandQueue.push(&turnRightCommand);
}

void GameplayState::render() {
    engine_.render();
}