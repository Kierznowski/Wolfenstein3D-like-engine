#include "Game/MainMenuState.h"

#include "Engine/Engine.h"
#include "Engine/Renderer.h"
//#include "Game/GameplayState.h"

MainMenuState::MainMenuState() {

}

void MainMenuState::handleInput(Engine& engine) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            finished = true;
        }
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
                        finished = true;
                    }
                    if (selected == 1) {
                        // TODO: networking menu
                    }
                    if (selected == 2) {
                        finished = true;
                    }
                    break;
                default: ;
            }
        }
    }
}

void MainMenuState::update(Engine& engine, double dt) {

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

bool MainMenuState::isFinished() const {
    return finished;
}

std::unique_ptr<GameState> MainMenuState::nextState() {
    //if (selected == 0) return std::make_unique<GameplayState>();
    if (selected == 2) return nullptr;
    return std::make_unique<MainMenuState>();
}




