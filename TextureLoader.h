#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "raylib.h"
#include <string>

// Constants
const int FALLBACK_TEXTURE_SIZE = 32;

// Texture loading with fallback
Texture2D LoadPlayerTexture(const std::string& path);

#endif // TEXTURE_LOADER_H