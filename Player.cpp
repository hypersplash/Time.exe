#include "Player.h"

Player::Player(float x, float y, float playerSpeed, float playerFriction, float playerMaxSpeed, Texture2D playerTexture) 
    : position{x, y}, velocity{0.0f, 0.0f}, speed(playerSpeed), friction(playerFriction), 
      maxSpeed(playerMaxSpeed), texture(playerTexture) {
    acceleration = speed * 25.0f;
}

void Player::handleInput(float deltaTime) {
    // Input handling
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) velocity.x += acceleration * deltaTime;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) velocity.x -= acceleration * deltaTime;
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) velocity.y += acceleration * deltaTime;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) velocity.y -= acceleration * deltaTime;
}

void Player::update(float deltaTime, int screenWidth, int screenHeight) {
    // Apply friction
    velocity.x -= velocity.x * friction * deltaTime;
    velocity.y -= velocity.y * friction * deltaTime;
    
    // Clamp speed
    if (velocity.x > maxSpeed) velocity.x = maxSpeed;
    if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;
    if (velocity.y > maxSpeed) velocity.y = maxSpeed;
    if (velocity.y < -maxSpeed) velocity.y = -maxSpeed;
    
    // Update position
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    
    // Keep inside screen
    clampToScreen(screenWidth, screenHeight);
}

void Player::clampToScreen(int screenWidth, int screenHeight) {
    float playerWidth = static_cast<float>(texture.width);
    float playerHeight = static_cast<float>(texture.height);
    float screenWidthF = static_cast<float>(screenWidth);
    float screenHeightF = static_cast<float>(screenHeight);
    
    if (position.x < 0) position.x = 0;
    if (position.y < 0) position.y = 0;
    if (position.x > screenWidthF - playerWidth) position.x = screenWidthF - playerWidth;
    if (position.y > screenHeightF - playerHeight) position.y = screenHeightF - playerHeight;
}

void Player::draw() {
    DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}