#pragma once
#include "GameState.h"

class MainMenuState : public GameState {
public:
    MainMenuState();

    void handleInput(Engine& engine) override;
    void update(Engine& engine, double dt) override;
    void render(Engine& engine) override;

    bool isFinished() const override;
    std::unique_ptr<GameState> nextState() override;

private:
    int selected = 0;
    bool finished = false;
};
