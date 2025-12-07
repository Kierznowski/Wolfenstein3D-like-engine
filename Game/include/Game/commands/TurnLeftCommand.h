#pragma once
#include "Engine/Command.h"
#include "Engine/Player.h"

class TurnLeftCommand : public Command {
public:
    void execute(Player& player, double dt) override {
        player.turnLeft(dt);
    }
};