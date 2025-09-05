#ifndef COMMANDS_H
#define COMMANDS_H

#include "Command.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// The CommandParser class is responsible for parsing user input and executing the corresponding commands.
class CommandParser {
public:
    // Constructor that initializes the command parser and registers the available commands.
    CommandParser();
    // Parses the given input string and executes the corresponding command.
    void parseAndExecute(const std::string& input, Player& player);

private:
    // A map that stores the registered commands, mapping command names to Command objects.
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;
};

#endif // COMMANDS_H