#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// The ScreenBounds struct holds the calculated boundaries of the screen.
// This is used to prevent the player from moving off-screen.
struct ScreenBounds {
    float maxX = 0.0f; // The maximum x-coordinate the player can have.
    float maxY = 0.0f; // The maximum y-coordinate the player can have.
    int lastWidth = -1; // The last known screen width, used to detect changes.
    int lastHeight = -1; // The last known screen height, used to detect changes.
    
    // Updates the screen boundaries.
    void update(int screenWidth, int screenHeight, float textureWidth, float textureHeight);
    // Checks if the screen dimensions have changed.
    bool needsUpdate(int screenWidth, int screenHeight) const;
};

// The Player struct represents the player character in the game.
// It manages the player's position, movement, and appearance.
struct Player {
    Vector2 position;    // The player's current position.
    Vector2 velocity;    // The player's current velocity.
    float speed;         // The player's current movement speed.
    float baseSpeed;     // The player's base movement speed.
    float friction;      // The friction applied to the player's movement.
    float maxSpeed;      // The maximum speed the player can reach.
    float baseMaxSpeed;  // The player's base maximum speed.
    float acceleration;  // The player's acceleration.
    Texture2D texture;   // The player's texture.
    
    // Cached screen boundaries to avoid recalculating them every frame.
    mutable ScreenBounds bounds;
    
    // Player constructor.
    Player(float x, float y, float playerSpeed, float playerFriction, 
           float playerMaxSpeed, Texture2D playerTexture);
    
    // Updates the player's state.
    void update(float deltaTime, int screenWidth, int screenHeight, bool consoleActive);
    // Handles player input.
    void handleInput(float deltaTime, bool consoleActive);
    // Draws the player on the screen.
    void draw() const;
    
private:
    // Applies physics to the player.
    void applyPhysics(float deltaTime);
    // Clamps the player's position to the screen boundaries.
    void clampToScreen(int screenWidth, int screenHeight);
};

#endif // PLAYER_H
