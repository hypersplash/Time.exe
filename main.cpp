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
    // Initializes the console with welcome messages.
    void initializeConsole() {
        consoleCapture.addLine("Game started successfully");
        consoleCapture.addLine("Config loaded successfully (resources/conf.ini)");
    }
    
    // Creates the player object, positioning it in the center of the screen.
    Player createPlayer(const GameConfig& config, Texture2D texture) {
        const float startX = static_cast<float>(config.screenWidth) / 2.0f - 
                            static_cast<float>(texture.width) / 2.0f;
        const float startY = static_cast<float>(config.screenHeight) / 2.0f - 
                            static_cast<float>(texture.height) / 2.0f;
        
        return Player(startX, startY, config.playerSpeed, 
                     config.friction, config.maxSpeed, texture);
    }
    
    // Updates the game state, handling all input and game logic.
    void updateGame(Player& player, GameState& gameState, const GameConfig& config, float deltaTime, CommandParser& commandParser, ConsoleInput& consoleInput) {
        // Handle input based on the current game state.
        if (gameState.isOnTitleScreen()) {
            gameState.handleTitleInput();
        } else {
            // Toggle console input with ALT+C.
            if (gameState.consoleVisible && IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_C)) {
                consoleInput.active = !consoleInput.active;
            }

            // If console input is active, process text input.
            if (consoleInput.active) {
                int key = GetCharPressed();
                if (key >= 32 && key <= 125) {
                    consoleInput.text.insert(consoleInput.cursorPosition, 1, (char)key);
                    consoleInput.cursorPosition++;
                }

                // Handle backspace for deleting characters.
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (consoleInput.cursorPosition > 0) {
                        consoleInput.text.erase(consoleInput.cursorPosition - 1, 1);
                        consoleInput.cursorPosition--;
                    }
                }

                // Handle ENTER to execute the command.
                if (IsKeyPressed(KEY_ENTER)) {
                    commandParser.parseAndExecute(consoleInput.text, player);
                    consoleInput.text.clear();
                    consoleInput.cursorPosition = 0;
                }
            } else {
                // Otherwise, handle normal game input.
                gameState.handleGameInput(config.consoleEnabled);
            }

            // Update the player only when the game is in the PLAYING state.
            if (gameState.isInGame()) {
                player.update(deltaTime, config.screenWidth, config.screenHeight, consoleInput.active);
            }
        }
    }
    
    // Renders the entire game, including the world, UI, and console.
    void renderGame(const Player& player, const GameConfig& config, const GameState& gameState, const ConsoleInput& consoleInput) {
        BeginDrawing();
        
        if (gameState.isOnTitleScreen()) {
            // Render the title screen.
            DrawTitleScreen(config.screenWidth, config.screenHeight);
        } else {
            // Render the main game world.
            ClearBackground(GRAY);
            
            // Draw the player.
            player.draw();
            
            // Draw the FPS counter if enabled.
            if (config.showFPS) {
                DrawFPS(10, 10);
            }
            
            // Draw the console if it's enabled and visible.
            if (config.consoleEnabled && gameState.consoleVisible) {
                DrawConsole(config.showFPS, config.consoleWidth, 
                           config.consoleHeight, config.consoleFontSize, consoleInput);
            }
            
            // Draw the pause screen if the game is paused.
            if (gameState.isPaused()) {
                DrawPauseScreen(config.screenWidth, config.screenHeight);
            }
        }
        
        EndDrawing();
    }
}

int main() {
    // Load the game configuration from the INI file.
    const auto configData = parseINI("resources/conf.ini");
    GameConfig config;
    config.loadFromConfig(configData);
    
    // Set the maximum number of characters per line for the console.
    consoleCapture.setMaxDisplayChars(config.calculateMaxDisplayChars());
    
    // Initialize the game window and set the target FPS.
    InitWindow(config.screenWidth, config.screenHeight, WINDOW_TITLE);
    SetExitKey(KEY_NULL); // Disable the default ESC key for exiting.
    SetTargetFPS(60);
    
    // Initialize game components.
    initializeConsole();
    
    const Texture2D playerTexture = LoadPlayerTexture(config.spritePath);
    Player player = createPlayer(config, playerTexture);
    GameState gameState;  // The game starts on the title screen by default.
    CommandParser commandParser;
    ConsoleInput consoleInput;
    
    // The main game loop.
    while (!WindowShouldClose() && !gameState.shouldQuit) {
        const float deltaTime = GetFrameTime();
        
        // Update all game logic.
        updateGame(player, gameState, config, deltaTime, commandParser, consoleInput);
        
        // Render everything to the screen.
        renderGame(player, config, gameState, consoleInput);
    }
    
    // Clean up resources before exiting.
    UnloadTexture(playerTexture);
    CloseWindow();
    return 0;
}