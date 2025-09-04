#include "raylib.h"
#include "ConsoleCapture.h"
#include "ConfigParser.h"
#include "TextureLoader.h"
#include "UIRenderer.h"
#include "Player.h"
#include "GameConfig.h"
#include "GameState.h"
#include "Version.h"
#include "Commands.h"

namespace {
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
    
    void updateGame(Player& player, GameState& gameState, const GameConfig& config, float deltaTime, CommandParser& commandParser, ConsoleInput& consoleInput) {
    // Handle input based on current state
    if (gameState.isOnTitleScreen()) {
        gameState.handleTitleInput();
    } else {
        if (gameState.consoleVisible && IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_C)) {
            consoleInput.active = !consoleInput.active;
        }

        if (consoleInput.active) {
            int key = GetCharPressed();
            if (key >= 32 && key <= 125) {
                consoleInput.text.insert(consoleInput.cursorPosition, 1, (char)key);
                consoleInput.cursorPosition++;
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (consoleInput.cursorPosition > 0) {
                    consoleInput.text.erase(consoleInput.cursorPosition - 1, 1);
                    consoleInput.cursorPosition--;
                }
            }

            if (IsKeyPressed(KEY_ENTER)) {
                commandParser.parseAndExecute(consoleInput.text, player);
                consoleInput.text.clear();
                consoleInput.cursorPosition = 0;
            }
        } else {
            gameState.handleGameInput(config.consoleEnabled);
        }

        // Update player only when actually playing (not paused)
        if (gameState.isInGame()) {
            player.update(deltaTime, config.screenWidth, config.screenHeight, consoleInput.active);
        }
    }
}
    
    void renderGame(const Player& player, const GameConfig& config, const GameState& gameState, const ConsoleInput& consoleInput) {
        BeginDrawing();
        
        if (gameState.isOnTitleScreen()) {
            // Render title screen
            DrawTitleScreen(config.screenWidth, config.screenHeight);
        } else {
            // Render game world
            ClearBackground(GRAY);
            
            // Draw player
            player.draw();
            
            // Draw FPS if enabled
            if (config.showFPS) {
                DrawFPS(10, 10);
            }
            
            // Draw console if enabled and visible
            if (config.consoleEnabled && gameState.consoleVisible) {
                DrawConsole(config.showFPS, config.consoleWidth, 
                           config.consoleHeight, config.consoleFontSize, consoleInput);
            }
            
            // Draw pause screen
            if (gameState.isPaused()) {
                DrawPauseScreen(config.screenWidth, config.screenHeight);
            }
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
    InitWindow(config.screenWidth, config.screenHeight, WINDOW_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    
    // Initialize game components
    initializeConsole();
    
    const Texture2D playerTexture = LoadPlayerTexture(config.spritePath);
    Player player = createPlayer(config, playerTexture);
    GameState gameState;  // Starts in TITLE_SCREEN state
    CommandParser commandParser;
    ConsoleInput consoleInput;
    
    // Main game loop
    while (!WindowShouldClose() && !gameState.shouldQuit) {
        const float deltaTime = GetFrameTime();
        
        // Update game state and logic
        updateGame(player, gameState, config, deltaTime, commandParser, consoleInput);
        
        // Render everything
        renderGame(player, config, gameState, consoleInput);
    }
    
    // Cleanup
    UnloadTexture(playerTexture);
    CloseWindow();
    return 0;
}