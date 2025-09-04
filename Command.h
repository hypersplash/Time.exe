#ifndef COMMAND_H
#define COMMAND_H

#include "Player.h"
#include <string>
#include <vector>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const std::vector<std::string>& args, Player& player) = 0;
};

#endif // COMMAND_H