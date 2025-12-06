#pragma once
#include <memory>

class Engine;

class GameState {
public:
    ~GameState() = default;

    virtual void handleInput(Engine& engine) = 0;
    virtual void update(Engine& engine, double dt) = 0;
    virtual void render(Engine& engine) = 0;

    virtual bool isFinished() const {
        return false;
    }

    virtual std::unique_ptr<GameState> nextState() {
        return nullptr;
    }

};