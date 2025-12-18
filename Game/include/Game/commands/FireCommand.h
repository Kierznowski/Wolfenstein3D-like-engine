#pragma once
#include "Engine/Command/Command.h"

class FireCommand : public Command {
public:
    void execute(Player& player, double dt) const override {
        player.requestFire();
    }
};