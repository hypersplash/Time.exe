#include "raylib.h"
#include "ConsoleCapture.h"
#include "ConfigParser.h"
#include "TextureLoader.h"
#include "UIRenderer.h"
#include "Player.h"
#include "GameConfig.h"

namespace {
    // Game state management
    struct GameState {
        bool consoleVisible = false;
        bool isPaused = false;
        bool shouldQuit = false;
        
        void handleInput(bool consoleEnabled) {
            // Console toggle (grave/tilde key)
            if (consoleEnabled && IsKeyPressed(KEY_GRAVE)) {
                consoleVisible = !consoleVisible;
            }
            
            // Pause toggle (ESC key)
            if (IsKeyPressed(KEY_ESCAPE)) {
                isPaused = !isPaused;
            }
            
            // Quit from pause screen
            if (isPaused && IsKeyPressed(KEY_Q)) {
                shouldQuit = true;
            }
        }
    };
    
    void initializeConsole() {
        consoleCapture.addLine("Game started successfully");
        consoleCapture.addLine("Config loaded successfully (resources/conf.ini)");
    }
    
    Player createPlayer(const GameConfig& config, Texture2D texture) {
        const float startX = static_cast<float>(config.screenWidth) / 2.0f - 
                            static_cast<float>(texture.width) / 2.0f;
        const float startY = static_cast<float>(config.screenHeight) / 2.0f - 
                            static_cast<float>(texture.height) / 2.0f;
        
        return Player(startX, startY, config.playerSpeed, 
                     config.friction, config.maxSpeed, texture);
    }
    
    void renderGame(const Player& player, const GameConfig& config, const GameState& state) {
        BeginDrawing();
        ClearBackground(GRAY);
        
        // Draw player
        player.draw();
        
        // Draw FPS if enabled
        if (config.showFPS) {
            DrawFPS(10, 10);
        }
        
        // Draw console if enabled and visible
        if (config.consoleEnabled && state.consoleVisible) {
            DrawConsole(config.showFPS, config.consoleWidth, 
                       config.consoleHeight, config.consoleFontSize);
        }
        
        // Draw pause screen
        if (state.isPaused) {
            DrawPauseScreen(config.screenWidth, config.screenHeight);
        }
        
        EndDrawing();
    }
}

int main() {
    // Load and apply configuration
    const auto configData = parseINI("resources/conf.ini");
    GameConfig config;
    config.loadFromConfig(configData);
    
    // Set up console capture with calculated character limit
    consoleCapture.setMaxDisplayChars(config.calculateMaxDisplayChars());
    
    // Initialize window and disable default exit key
    InitWindow(config.screenWidth, config.screenHeight, "Abandoned Where? (TESTING MODE)");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    
    // Initialize game components
    initializeConsole();
    
    const Texture2D playerTexture = LoadPlayerTexture(config.spritePath);
    Player player = createPlayer(config, playerTexture);
    GameState gameState;
    
    // Main game loop
    while (!WindowShouldClose() && !gameState.shouldQuit) {
        const float deltaTime = GetFrameTime();
        
        // Handle input
        gameState.handleInput(config.consoleEnabled);
        
        // Update game logic only when not paused
        if (!gameState.isPaused) {
            player.update(deltaTime, config.screenWidth, config.screenHeight);
        }
        
        // Render everything
        renderGame(player, config, gameState);
    }
    
    // Cleanup
    UnloadTexture(playerTexture);
    CloseWindow();
    return 0;
}