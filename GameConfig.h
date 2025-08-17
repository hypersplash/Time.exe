#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <string>
#include <unordered_map>

struct GameConfig {
    // Window settings
    int screenWidth = 800;
    int screenHeight = 450;
    
    // Player settings
    float playerSpeed = 200.0f;
    float friction = 10.0f;
    float maxSpeed = 200.0f;
    std::string spritePath = "resources/test/testsprite.png";
    
    // Debug/Display settings
    bool showFPS = false;
    bool consoleEnabled = false;
    
    // Console settings
    int consoleFontSize = 14;
    int consoleWidth = 450;
    int consoleHeight = 280;
    
    // Load configuration from INI file
    void loadFromConfig(const std::unordered_map<std::string, std::string>& config);
    
    // Calculate max display characters for console
    int calculateMaxDisplayChars() const;

private:
    // Helper templates for type-safe config parsing
    template<typename T>
    void setConfigValue(const std::unordered_map<std::string, std::string>& config, 
                       const std::string& key, T& target);
    
    void setBoolConfig(const std::unordered_map<std::string, std::string>& config,
                      const std::string& key, bool& target);
};

#endif // GAME_CONFIG_H