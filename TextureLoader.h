#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "raylib.h"
#include <string>

// The size of the fallback texture to be generated if the player texture fails to load.
const int FALLBACK_TEXTURE_SIZE = 32;

// Declares the function to load the player texture.
// This function includes a fallback mechanism in case the texture file cannot be found or loaded.
Texture2D LoadPlayerTexture(const std::string& path);

#endif // TEXTURE_LOADER_H