#include "Player.h"
#include <algorithm>

// ScreenBounds implementation
// Updates the screen boundaries used for clamping player position.
void ScreenBounds::update(int screenWidth, int screenHeight, float textureWidth, float textureHeight) {
    maxX = static_cast<float>(screenWidth) - textureWidth;
    maxY = static_cast<float>(screenHeight) - textureHeight;
    lastWidth = screenWidth;
    lastHeight = screenHeight;
}

// Checks if the screen dimensions have changed, requiring an update to the boundaries.
bool ScreenBounds::needsUpdate(int screenWidth, int screenHeight) const {
    return lastWidth != screenWidth || lastHeight != screenHeight;
}

// Player implementation
Player::Player(float x, float y, float playerSpeed, float playerFriction, 
               float playerMaxSpeed, Texture2D playerTexture) 
    : position{x, y}, velocity{0.0f, 0.0f}, speed(playerSpeed), baseSpeed(playerSpeed), 
      friction(playerFriction), maxSpeed(playerMaxSpeed), baseMaxSpeed(playerMaxSpeed), texture(playerTexture) {
}

// Handles player input for movement.
void Player::handleInput(float deltaTime, bool consoleActive) {
    // Disable player movement when the console is active.
    if (consoleActive) return;
    acceleration = speed * 25.0f;
    const float accelDelta = acceleration * deltaTime;
    
    // Update velocity based on keyboard input.
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) velocity.x += accelDelta;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  velocity.x -= accelDelta;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  velocity.y += accelDelta;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    velocity.y -= accelDelta;
}

// Updates the player's state, including input, physics, and position.
void Player::update(float deltaTime, int screenWidth, int screenHeight, bool consoleActive) {
    handleInput(deltaTime, consoleActive);
    applyPhysics(deltaTime);
    clampToScreen(screenWidth, screenHeight);
}

// Applies physics to the player, including friction and velocity clamping.
void Player::applyPhysics(float deltaTime) {
    const float frictionDelta = friction * deltaTime;
    
    // Apply friction to slow down the player.
    velocity.x -= velocity.x * frictionDelta;
    velocity.y -= velocity.y * frictionDelta;
    
    // Clamp the velocity to the maximum speed.
    velocity.x = std::clamp(velocity.x, -maxSpeed, maxSpeed);
    velocity.y = std::clamp(velocity.y, -maxSpeed, maxSpeed);
    
    // Update the player's position based on the velocity.
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

// Clamps the player's position to the screen boundaries.
void Player::clampToScreen(int screenWidth, int screenHeight) {
    // Recalculate screen boundaries if the window size has changed.
    if (bounds.needsUpdate(screenWidth, screenHeight)) {
        bounds.update(screenWidth, screenHeight, 
                     static_cast<float>(texture.width), 
                     static_cast<float>(texture.height));
    }
    
    // Clamp the player's position to stay within the screen.
    position.x = std::clamp(position.x, 0.0f, bounds.maxX);
    position.y = std::clamp(position.y, 0.0f, bounds.maxY);
}

// Draws the player on the screen.
void Player::draw() const {
    DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}