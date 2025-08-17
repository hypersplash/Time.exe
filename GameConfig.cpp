#include "GameConfig.h"

void GameConfig::loadFromConfig(const std::unordered_map<std::string, std::string>& config) {
    // Override with config values
    if (config.count("window_width")) screenWidth = std::stoi(config.at("window_width"));
    if (config.count("window_height")) screenHeight = std::stoi(config.at("window_height"));
    if (config.count("player_speed")) playerSpeed = std::stof(config.at("player_speed"));
    if (config.count("player_friction")) friction = std::stof(config.at("player_friction"));
    if (config.count("player_max_speed")) maxSpeed = std::stof(config.at("player_max_speed"));
    if (config.count("show_fps")) {
        std::string fpsValue = config.at("show_fps");
        showFPS = (fpsValue == "true" || fpsValue == "1" || fpsValue == "yes");
    }
    if (config.count("show_console")) {
        std::string consoleValue = config.at("show_console");
        consoleEnabled = (consoleValue == "true" || consoleValue == "1" || consoleValue == "yes");
    }
    if (config.count("console_font_size")) consoleFontSize = std::stoi(config.at("console_font_size"));
    if (config.count("console_width")) consoleWidth = std::stoi(config.at("console_width"));
    if (config.count("console_height")) consoleHeight = std::stoi(config.at("console_height"));
    if (config.count("player_sprite")) spritePath = config.at("player_sprite");
}

int GameConfig::calculateMaxDisplayChars() const {
    int estimatedCharWidth = static_cast<int>(consoleFontSize * 0.6f);
    int padding = consoleFontSize / 2;
    return (consoleWidth - (padding * 2)) / estimatedCharWidth;
}