#ifndef GAME_STATE_H
#define GAME_STATE_H

// Enum to represent the different states of the game.
enum class GameStateType {
    TITLE_SCREEN, // The game is on the title screen.
    PLAYING,      // The game is actively being played.
    PAUSED        // The game is paused.
};

// The GameState struct holds the current state of the game and provides methods to manage it.
struct GameState {
    GameStateType currentState = GameStateType::TITLE_SCREEN; // The current state of the game.
    bool consoleVisible = false; // Whether the developer console is visible.
    bool shouldQuit = false;     // Whether the game should quit.
    
    // Handles input when the game is on the title screen.
    void handleTitleInput();
    // Handles input during the main game loop (playing and paused states).
    void handleGameInput(bool consoleEnabled);
    // Sets the new game state.
    void setState(GameStateType newState);
    
    // Helper functions to check the current game state.
    bool isInGame() const { return currentState == GameStateType::PLAYING; }
    bool isPaused() const { return currentState == GameStateType::PAUSED; }
    bool isOnTitleScreen() const { return currentState == GameStateType::TITLE_SCREEN; }
};

#endif // GAME_STATE_H