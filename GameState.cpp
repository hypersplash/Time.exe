#include "GameState.h"
#include "raylib.h"

// Handles input when the game is on the title screen.
void GameState::handleTitleInput() {
    // Pressing SPACE starts the game.
    if (IsKeyPressed(KEY_SPACE)) {
        setState(GameStateType::PLAYING);
    }
    
    // Pressing ESCAPE quits the game.
    if (IsKeyPressed(KEY_ESCAPE)) {
        shouldQuit = true;
    }
}

// Handles input during the main game loop (playing and paused states).
void GameState::handleGameInput(bool consoleEnabled) {
    // Toggle the console visibility with the GRAVE key if the console is enabled.
    if (consoleEnabled && IsKeyPressed(KEY_GRAVE)) {
        consoleVisible = !consoleVisible;
    }
    
    // Toggle between playing and paused states with the ESCAPE key.
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == GameStateType::PLAYING) {
            setState(GameStateType::PAUSED);
        } else if (currentState == GameStateType::PAUSED) {
            setState(GameStateType::PLAYING);
        }
    }
    
    // Handle input specific to the paused state.
    if (currentState == GameStateType::PAUSED) {
        // Pressing Q quits the game.
        if (IsKeyPressed(KEY_Q)) {
            shouldQuit = true;
        }
        // Pressing T returns to the title screen.
        if (IsKeyPressed(KEY_T)) {
            setState(GameStateType::TITLE_SCREEN);
        }
    }
}

// Sets the new game state.
void GameState::setState(GameStateType newState) {
    currentState = newState;
    
    // Hide the console when returning to the title screen.
    if (newState == GameStateType::TITLE_SCREEN) {
        consoleVisible = false;
    }
}