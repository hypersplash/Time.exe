#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

struct Player {
    Vector2 position;
    Vector2 velocity;
    float speed;
    float friction;
    float maxSpeed;
    float acceleration;
    Texture2D texture;
    
    Player(float x, float y, float playerSpeed, float playerFriction, float playerMaxSpeed, Texture2D playerTexture);
    
    void update(float deltaTime, int screenWidth, int screenHeight);
    void handleInput(float deltaTime);
    void draw();
    void clampToScreen(int screenWidth, int screenHeight);
};

#endif // PLAYER_H