#pragma once
#include "Engine/Command.h"
#include "Engine/Player.h"

class TurnRightCommand : public Command {
public:
    void execute(Player& player, double dt) override {
        player.turnRight(dt);
    }
};