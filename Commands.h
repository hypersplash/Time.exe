#ifndef COMMANDS_H
#define COMMANDS_H

#include "Command.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class CommandParser {
public:
    CommandParser();
    void parseAndExecute(const std::string& input, Player& player);

private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;
};

#endif // COMMANDS_H