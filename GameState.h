#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class GameStateType {
    TITLE_SCREEN,
    PLAYING,
    PAUSED
};

struct GameState {
    GameStateType currentState = GameStateType::TITLE_SCREEN;
    bool consoleVisible = false;
    bool shouldQuit = false;
    
    void handleTitleInput();
    void handleGameInput(bool consoleEnabled);
    void setState(GameStateType newState);
    
    bool isInGame() const { return currentState == GameStateType::PLAYING; }
    bool isPaused() const { return currentState == GameStateType::PAUSED; }
    bool isOnTitleScreen() const { return currentState == GameStateType::TITLE_SCREEN; }
};

#endif // GAME_STATE_H