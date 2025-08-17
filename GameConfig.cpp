#include "GameConfig.h"
#include <stdexcept>

void GameConfig::loadFromConfig(const std::unordered_map<std::string, std::string>& config) {
    // Single lookup per config value - much more efficient than count() + at()
    setConfigValue(config, "window_width", screenWidth);
    setConfigValue(config, "window_height", screenHeight);
    setConfigValue(config, "player_speed", playerSpeed);
    setConfigValue(config, "player_friction", friction);
    setConfigValue(config, "player_max_speed", maxSpeed);
    setConfigValue(config, "console_font_size", consoleFontSize);
    setConfigValue(config, "console_width", consoleWidth);
    setConfigValue(config, "console_height", consoleHeight);
    
    // Boolean configs need special handling
    setBoolConfig(config, "show_fps", showFPS);
    setBoolConfig(config, "show_console", consoleEnabled);
    
    // String config
    if (auto it = config.find("player_sprite"); it != config.end()) {
        spritePath = it->second;
    }
}

int GameConfig::calculateMaxDisplayChars() const {
    const float charWidthRatio = 0.6f;
    const int estimatedCharWidth = static_cast<int>(consoleFontSize * charWidthRatio);
    const int padding = consoleFontSize / 2;
    const int usableWidth = consoleWidth - (padding * 2);
    
    return std::max(1, usableWidth / estimatedCharWidth);  // Ensure at least 1 char
}

template<typename T>
void GameConfig::setConfigValue(const std::unordered_map<std::string, std::string>& config,
                               const std::string& key, T& target) {
    if (auto it = config.find(key); it != config.end()) {
        try {
            if constexpr (std::is_same_v<T, int>) {
                target = std::stoi(it->second);
            } else if constexpr (std::is_same_v<T, float>) {
                target = std::stof(it->second);
            }
        } catch (const std::exception&) {
            // Silently ignore invalid values, keep defaults
            // Could log this if needed
        }
    }
}

void GameConfig::setBoolConfig(const std::unordered_map<std::string, std::string>& config,
                              const std::string& key, bool& target) {
    if (auto it = config.find(key); it != config.end()) {
        const std::string& value = it->second;
        target = (value == "true" || value == "1" || value == "yes");
    }
}