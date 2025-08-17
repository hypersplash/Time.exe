#include "raylib.h"
#include "ConsoleCapture.h"
#include "ConfigParser.h"
#include "TextureLoader.h"
#include "UIRenderer.h"
#include "Player.h"
#include "GameConfig.h"

int main()
{
    // --- Load config file ---
    auto conf = parseINI("resources/conf.ini");
    
    // Initialize game configuration
    GameConfig gameConfig;
    gameConfig.loadFromConfig(conf);
    
    // Set up console capture
    consoleCapture.setMaxDisplayChars(gameConfig.calculateMaxDisplayChars());

    // Initialize window
    InitWindow(gameConfig.screenWidth, gameConfig.screenHeight, "Abandoned Where? (TESTING MODE)");
    SetExitKey(KEY_NULL); // Disable default exit key (ESC) to handle it manually
    
    // Add some example console messages
    consoleCapture.addLine("Game started successfully");
    consoleCapture.addLine("Config loaded successfully (resources/conf.ini)");
    
    // Load player texture
    Texture2D playerTexture = LoadPlayerTexture(gameConfig.spritePath);
    
    // Create player
    float startX = static_cast<float>(gameConfig.screenWidth) / 2.0f - static_cast<float>(playerTexture.width) / 2.0f;
    float startY = static_cast<float>(gameConfig.screenHeight) / 2.0f - static_cast<float>(playerTexture.height) / 2.0f;
    Player player(startX, startY, gameConfig.playerSpeed, gameConfig.friction, gameConfig.maxSpeed, playerTexture);
    
    // Game state variables
    bool consoleVisible = false;
    bool isPaused = false;
    bool shouldQuit = false;
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose() && !shouldQuit)
    {
        float deltaTime = GetFrameTime();
        
        // --- Console toggle (grave/tilde key) ---
        if (gameConfig.consoleEnabled && IsKeyPressed(KEY_GRAVE)) {
            consoleVisible = !consoleVisible;
        }
        
        // --- Pause toggle (ESC key) ---
        if (IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
        }
        
        // --- Quit from pause screen ---
        if (isPaused && IsKeyPressed(KEY_Q)) {
            shouldQuit = true;
        }
        
        // Only update game logic if not paused
        if (!isPaused) {
            player.handleInput(deltaTime);
            player.update(deltaTime, gameConfig.screenWidth, gameConfig.screenHeight);
        }
        
        // --- Draw ---
        BeginDrawing();
        ClearBackground(GRAY);
        
        // Draw player
        player.draw();
        
        // --- Draw FPS if enabled ---
        if (gameConfig.showFPS) {
            DrawFPS(10, 10);
        }
        
        // --- Draw Console if enabled and visible ---
        if (gameConfig.consoleEnabled && consoleVisible) {
            DrawConsole(gameConfig.showFPS, gameConfig.consoleWidth, gameConfig.consoleHeight, gameConfig.consoleFontSize);
        }
        
        // --- Draw Pause Screen ---
        if (isPaused) {
            DrawPauseScreen(gameConfig.screenWidth, gameConfig.screenHeight);
        }
        
        EndDrawing();
    }
    
    UnloadTexture(playerTexture);
    CloseWindow();
    return 0;
}