#pragma once

#include "Command.h"

class CommandQueue {
public:
    CommandQueue() = default;

    void push(const Command* command) {
        queue_.emplace_back(command);
    }

    std::vector<const Command*>& getQueue() {
        return queue_;
    }

    void clear() {
        queue_.clear();
    }

private:
    std::vector<const Command*> queue_;
};
