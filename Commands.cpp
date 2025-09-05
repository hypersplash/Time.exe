#include "Commands.h"
#include "Player.h"
#include "ConsoleCapture.h"
#include <iostream>
#include <sstream>
#include <vector>

// SpeedCommand implementation
// Defines a command to change the player's speed.
class SpeedCommand : public Command {
public:
    // Executes the speed command.
    // Takes a vector of string arguments and a reference to the Player object.
    void execute(const std::vector<std::string>& args, Player& player) override {
        // Check if the correct number of arguments is provided.
        if (args.size() != 1) {
            consoleCapture.addLine("SPEED: No arguments");
            return;
        }
        try {
            // Convert the argument to a float to use as a speed multiplier.
            float speedMultiplier = std::stof(args[0]);
            // Update player's speed and max speed based on the multiplier.
            player.speed = player.baseSpeed * speedMultiplier;
            player.maxSpeed = player.baseMaxSpeed * speedMultiplier;
            // Log the new speed to the console.
            consoleCapture.addLine("CL: Speed set to " + std::to_string(static_cast<int>(player.speed)));
        } catch (const std::invalid_argument& e) {
            // Handle cases where the argument is not a valid number. (NUMBERS BABY NUMBERS!)
            consoleCapture.addLine("CL: NUMERS BABY NUMBERS"); // I seriously don't know why I did this as a reference to the Apollo lmao
        }
    }
};

// CommandParser implementation
// Manages and processes registered commands.
CommandParser::CommandParser() {
    // Register the "speed" command.
    commands["speed"] = std::make_unique<SpeedCommand>();
}

// Parses a command string and executes the corresponding command.
void CommandParser::parseAndExecute(const std::string& input, Player& player) {
    // Log the command to the console.
    consoleCapture.addLine("> " + input);
    std::stringstream ss(input);
    std::string commandName;
    // Extract the command name from the input string.
    ss >> commandName;

    // Extract the arguments from the input string.
    std::vector<std::string> args;
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }

    // Find the command in the commands map.
    auto it = commands.find(commandName);
    if (it != commands.end()) {
        // If found, execute the command with the provided arguments.
        it->second->execute(args, player);
    } else {
        // If not found, log an error message.
        consoleCapture.addLine("CL: Illegal command");
    }
}