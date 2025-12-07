#pragma once

#include "Command.h"

class CommandQueue {
public:
    CommandQueue() = default;

    void push(std::unique_ptr<Command> command) {
        queue.emplace_back(std::move(command));
    }

    std::vector<std::unique_ptr<Command>>& getQueue() {
        return queue;
    }

    void clear() {
        queue.clear();
    }

private:
    std::vector<std::unique_ptr<Command>> queue;
};
