#include "Player.h"
#include <algorithm>

// ScreenBounds implementation
void ScreenBounds::update(int screenWidth, int screenHeight, float textureWidth, float textureHeight) {
    maxX = static_cast<float>(screenWidth) - textureWidth;
    maxY = static_cast<float>(screenHeight) - textureHeight;
    lastWidth = screenWidth;
    lastHeight = screenHeight;
}

bool ScreenBounds::needsUpdate(int screenWidth, int screenHeight) const {
    return lastWidth != screenWidth || lastHeight != screenHeight;
}

// Player implementation
Player::Player(float x, float y, float playerSpeed, float playerFriction, 
               float playerMaxSpeed, Texture2D playerTexture) 
    : position{x, y}, velocity{0.0f, 0.0f}, speed(playerSpeed), 
      friction(playerFriction), maxSpeed(playerMaxSpeed), texture(playerTexture) {
    
    // Pre-calculate acceleration once
    acceleration = speed * 25.0f;
}

void Player::handleInput(float deltaTime) {
    const float accelDelta = acceleration * deltaTime;
    
    // Handle input with early returns for better branch prediction
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) velocity.x += accelDelta;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  velocity.x -= accelDelta;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  velocity.y += accelDelta;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    velocity.y -= accelDelta;
}

void Player::update(float deltaTime, int screenWidth, int screenHeight) {
    handleInput(deltaTime);
    applyPhysics(deltaTime);
    clampToScreen(screenWidth, screenHeight);
}

void Player::applyPhysics(float deltaTime) {
    const float frictionDelta = friction * deltaTime;
    
    // Apply friction
    velocity.x -= velocity.x * frictionDelta;
    velocity.y -= velocity.y * frictionDelta;
    
    // Clamp velocity to max speed using std::clamp (cleaner than if statements)
    velocity.x = std::clamp(velocity.x, -maxSpeed, maxSpeed);
    velocity.y = std::clamp(velocity.y, -maxSpeed, maxSpeed);
    
    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Player::clampToScreen(int screenWidth, int screenHeight) {
    // Only recalculate bounds when screen size changes
    if (bounds.needsUpdate(screenWidth, screenHeight)) {
        bounds.update(screenWidth, screenHeight, 
                     static_cast<float>(texture.width), 
                     static_cast<float>(texture.height));
    }
    
    // Clamp position to screen boundaries
    position.x = std::clamp(position.x, 0.0f, bounds.maxX);
    position.y = std::clamp(position.y, 0.0f, bounds.maxY);
}

void Player::draw() const {
    DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}