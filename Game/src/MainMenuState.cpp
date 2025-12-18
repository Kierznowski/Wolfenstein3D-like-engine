#include "Game/MainMenuState.h"

#include "../../Engine/include/Engine/renderer/Renderer.h"
#include "Game/GameplayState.h"


void MainMenuState::handleInput(const SDL_Event& event, CommandQueue& commandQueue) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selected = (selected + 2) % 3;
                break;
            case SDLK_DOWN:
                selected = (selected + 1) % 3;
                break;
            case SDLK_RETURN:
                if (selected == 0) {
                    engine.setState(std::make_unique<GameplayState>(engine));
                } else if (selected == 2) {
                    engine.stop();
                }
                break;
            default: ;
        }
    }
}

void MainMenuState::handleRealtimeInput(CommandQueue& command_queue) {
}

void MainMenuState::render() {
    Renderer* renderer = engine.getRenderer();
    renderer->clear();

    renderer->drawText(50, 50, "NET-2.5D-SHOOTER-GAME");
    renderer->drawText(50, 150, (selected == 0 ? "> Create server" : " Create server"));
    renderer->drawText(50, 200, (selected == 1 ? "> Connect to server" : " Connect to server"));
    renderer->drawText(50, 250, (selected == 2 ? "> Exit" : " Exit"));

    renderer->presentFrame();
}