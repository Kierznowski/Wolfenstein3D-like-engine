#pragma once
#include "../../../../Engine/include/Engine/Command/Command.h"
#include "../../../../Engine/include/Engine/Player.h"

class MoveForwardCommand : public Command {
public:
    void execute(Player& player, const double dt) const override {
        player.moveForward(dt);
    }
};