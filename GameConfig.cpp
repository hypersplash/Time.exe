#include "GameConfig.h"
#include <stdexcept>

// Loads the game configuration from a given map of key-value pairs.
void GameConfig::loadFromConfig(const std::unordered_map<std::string, std::string>& config) {
    // Set various configuration values using a helper function.
    setConfigValue(config, "window_width", screenWidth);
    setConfigValue(config, "window_height", screenHeight);
    setConfigValue(config, "player_speed", playerSpeed);
    setConfigValue(config, "player_friction", friction);
    setConfigValue(config, "player_max_speed", maxSpeed);
    setConfigValue(config, "console_font_size", consoleFontSize);
    setConfigValue(config, "console_width", consoleWidth);
    setConfigValue(config, "console_height", consoleHeight);
    
    // Handle boolean configuration values separately.
    setBoolConfig(config, "show_fps", showFPS);
    setBoolConfig(config, "show_console", consoleEnabled);
    
    // Handle string configuration values.
    if (auto it = config.find("player_sprite"); it != config.end()) {
        spritePath = it->second;
    }
}

// Calculates the maximum number of characters that can be displayed in a single console line.
int GameConfig::calculateMaxDisplayChars() const {
    const float charWidthRatio = 0.6f; // Estimated ratio of character width to font size.
    const int estimatedCharWidth = static_cast<int>(consoleFontSize * charWidthRatio);
    const int padding = consoleFontSize / 2;
    const int usableWidth = consoleWidth - (padding * 2);
    
    // Ensure that at least one character can be displayed.
    return std::max(1, usableWidth / estimatedCharWidth);
}

// Template function to set a configuration value of a given type.
// This avoids code duplication for different numeric types.
template<typename T>
void GameConfig::setConfigValue(const std::unordered_map<std::string, std::string>& config,
                               const std::string& key, T& target) {
    if (auto it = config.find(key); it != config.end()) {
        try {
            // Use `if constexpr` to handle different types at compile time.
            if constexpr (std::is_same_v<T, int>) {
                target = std::stoi(it->second);
            } else if constexpr (std::is_same_v<T, float>) {
                target = std::stof(it->second);
            }
        } catch (const std::exception&) {
            // Silently ignore invalid values and keep the default.
            // A log message could be added here for debugging purposes.
        }
    }
}

// Sets a boolean configuration value based on a string representation.
void GameConfig::setBoolConfig(const std::unordered_map<std::string, std::string>& config,
                              const std::string& key, bool& target) {
    if (auto it = config.find(key); it != config.end()) {
        const std::string& value = it->second;
        // Treat "true", "1", and "yes" as true, and anything else as false.
        target = (value == "true" || value == "1" || value == "yes");
    }
}