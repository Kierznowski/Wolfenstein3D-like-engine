#pragma once

#include "../Player.h"

struct Command {
    virtual ~Command() = default;
    virtual void execute(Player& player, double dt) const = 0;
};