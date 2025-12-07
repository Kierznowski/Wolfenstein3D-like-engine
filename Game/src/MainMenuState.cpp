#include "Game/MainMenuState.h"

#include "Engine/Renderer.h"
#include "Game/GameplayState.h"


void MainMenuState::handleInput(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selected = (selected + 2) % 3;
                break;
            case SDLK_DOWN:
                selected = (selected + 1) % 3;
                break;
            case SDLK_RETURN:
                nextState = std::make_unique<GameplayState>();
                isFinished = true;
                break;
            default: ;
        }
    }
}

void MainMenuState::handleInput(CommandQueue &commandQueue) {

}

void MainMenuState::render(Engine &engine) {
    Renderer& renderer = engine.getRenderer();
    renderer.clear();

    renderer.drawText(50, 50, "RAYCASTER ENGINE MENU");
    renderer.drawText(50, 150, (selected == 0 ? "> Start Game" : " Start Game"));
    renderer.drawText(50, 200, (selected == 1 ? "> Multiplayer" : " Multiplayer"));
    renderer.drawText(50, 250, (selected == 2 ? "> Exit" : " Exit"));

    renderer.presentFrame();
}