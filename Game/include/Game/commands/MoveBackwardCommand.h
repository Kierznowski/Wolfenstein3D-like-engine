#pragma once
#include "Engine/Command/Command.h"

class MoveBackwardCommand : public Command {
public:
    void execute(Player& player, const double dt) const override {
        player.moveBackward(dt);
    }
};