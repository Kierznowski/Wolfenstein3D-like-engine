#pragma once
#include "Engine/Command.h"
#include "Engine/Player.h"

class MoveBackwardCommand : public Command {
public:
    void execute(Player& player, double dt) override {
        player.moveBackward(dt);
    }
};