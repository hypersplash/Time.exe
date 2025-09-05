#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <string>
#include <unordered_map>

// The GameConfig struct holds all the configuration settings for the game.
// It provides default values and can be loaded from a configuration file.
struct GameConfig {
    // Window settings
    int screenWidth = 800;          // The width of the game window.
    int screenHeight = 450;         // The height of the game window.
    
    // Player settings
    float playerSpeed = 200.0f;     // The movement speed of the player.
    float friction = 10.0f;         // The friction applied to the player's movement.
    float maxSpeed = 200.0f;        // The maximum speed the player can reach.
    std::string spritePath = "resources/test/testsprite.png"; // The path to the player's sprite.
    
    // Debug/Display settings
    bool showFPS = false;           // Whether to display the FPS counter.
    bool consoleEnabled = false;    // Whether the developer console is enabled.
    
    // Console settings
    int consoleFontSize = 14;       // The font size used in the console.
    int consoleWidth = 450;         // The width of the console window.
    int consoleHeight = 280;        // The height of the console window.
    
    // Loads the configuration from a map of key-value pairs.
    void loadFromConfig(const std::unordered_map<std::string, std::string>& config);
    
    // Calculates the maximum number of characters that can be displayed in a single console line.
    int calculateMaxDisplayChars() const;

private:
    // A template helper function to set a configuration value of a given type.
    template<typename T>
    void setConfigValue(const std::unordered_map<std::string, std::string>& config, 
                       const std::string& key, T& target);
    
    // A helper function to set a boolean configuration value.
    void setBoolConfig(const std::unordered_map<std::string, std::string>& config,
                      const std::string& key, bool& target);
};

#endif // GAME_CONFIG_H