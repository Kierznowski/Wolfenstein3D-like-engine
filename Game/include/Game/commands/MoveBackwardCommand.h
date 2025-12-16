#pragma once
#include "../../../../Engine/include/Engine/Command/Command.h"
#include "../../../../Engine/include/Engine/Player.h"

class MoveBackwardCommand : public Command {
public:
    void execute(Player& player, double dt) const override {
        player.moveBackward(dt);
    }
};