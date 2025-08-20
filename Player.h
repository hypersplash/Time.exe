#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct ScreenBounds {
    float maxX = 0.0f;
    float maxY = 0.0f;
    int lastWidth = -1;
    int lastHeight = -1;
    
    void update(int screenWidth, int screenHeight, float textureWidth, float textureHeight);
    bool needsUpdate(int screenWidth, int screenHeight) const;
};

struct Player {
    Vector2 position;
    Vector2 velocity;
    float speed;
    float friction;
    float maxSpeed;
    float acceleration;
    Texture2D texture;
    
    // Cached screen boundaries for performance
    mutable ScreenBounds bounds;
    
    Player(float x, float y, float playerSpeed, float playerFriction, 
           float playerMaxSpeed, Texture2D playerTexture);
    
    void update(float deltaTime, int screenWidth, int screenHeight);
    void handleInput(float deltaTime);
    void draw() const;
    
private:
    void applyPhysics(float deltaTime);
    void clampToScreen(int screenWidth, int screenHeight);
};

#endif // PLAYER_H
