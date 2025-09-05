#ifndef COMMAND_H
#define COMMAND_H

#include "Player.h"
#include <string>
#include <vector>

// The Command class is an abstract base class for all commands in the game.
// It defines the interface for executing a command.
class Command {
public:
    // Virtual destructor.
    virtual ~Command() = default;
    // The pure virtual execute function that must be implemented by all derived classes.
    // It takes a vector of string arguments and a reference to the Player object.
    virtual void execute(const std::vector<std::string>& args, Player& player) = 0;
};

#endif // COMMAND_H