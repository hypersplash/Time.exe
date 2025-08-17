#include "GameState.h"
#include "raylib.h"

void GameState::handleTitleInput() {
    // Start game with space
    if (IsKeyPressed(KEY_SPACE)) {
        setState(GameStateType::PLAYING);
    }
    
    // Quit with escape
    if (IsKeyPressed(KEY_ESCAPE)) {
        shouldQuit = true;
    }
}

void GameState::handleGameInput(bool consoleEnabled) {
    // Console toggle (grave/tilde key)
    if (consoleEnabled && IsKeyPressed(KEY_GRAVE)) {
        consoleVisible = !consoleVisible;
    }
    
    // Pause toggle (ESC key)
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentState == GameStateType::PLAYING) {
            setState(GameStateType::PAUSED);
        } else if (currentState == GameStateType::PAUSED) {
            setState(GameStateType::PLAYING);
        }
    }
    
    // Quit from pause screen or return to title
    if (currentState == GameStateType::PAUSED) {
        if (IsKeyPressed(KEY_Q)) {
            shouldQuit = true;
        }
        if (IsKeyPressed(KEY_T)) {
            setState(GameStateType::TITLE_SCREEN);
        }
    }
}

void GameState::setState(GameStateType newState) {
    currentState = newState;
    
    // Reset console visibility when changing states
    if (newState == GameStateType::TITLE_SCREEN) {
        consoleVisible = false;
    }
}