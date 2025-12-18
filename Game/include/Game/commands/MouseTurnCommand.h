
#include "Engine/Command/Command.h"

class MouseTurnCommand : public Command {
public:
    explicit MouseTurnCommand(double magnitude)
        : magnitude {magnitude}
    {}

    void execute(Player& player, const double dt) const override {
        player.turnWithMouse(magnitude * SENSITIVITY * dt);
    }

private:
    double magnitude{};
    static constexpr double SENSITIVITY {0.15};
};
