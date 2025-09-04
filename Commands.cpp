#include "Commands.h"
#include "Player.h"
#include "ConsoleCapture.h"
#include <iostream>
#include <sstream>
#include <vector>

// SpeedCommand implementation
class SpeedCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, Player& player) override {
        if (args.size() != 1) {
            consoleCapture.addLine("Error: speed command requires one argument.");
            return;
        }
        try {
            float speedMultiplier = std::stof(args[0]);
            size_t pos;
            std::stof(args[0], &pos);
            player.speed = player.baseSpeed * speedMultiplier;
            player.maxSpeed = player.baseMaxSpeed * speedMultiplier;
            consoleCapture.addLine("CL: Speed set to " + std::to_string(static_cast<int>(player.speed)));
        } catch (const std::invalid_argument& e) {
            consoleCapture.addLine("CL: NUMERS BABY NUMBERS"); // I seriously don't know why I did this as a reference to the Apollo lmao
        }
    }
};

// CommandParser implementation
CommandParser::CommandParser() {
    commands["speed"] = std::make_unique<SpeedCommand>();
}

void CommandParser::parseAndExecute(const std::string& input, Player& player) {
    consoleCapture.addLine("> " + input);
    std::stringstream ss(input);
    std::string commandName;
    ss >> commandName;

    std::vector<std::string> args;
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }

    auto it = commands.find(commandName);
    if (it != commands.end()) {
        it->second->execute(args, player);
    } else {
        consoleCapture.addLine("Error: unknown command.");
    }
}