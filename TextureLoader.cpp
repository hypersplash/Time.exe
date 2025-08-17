#include "TextureLoader.h"
#include "ConsoleCapture.h"

Texture2D LoadPlayerTexture(const std::string& path) {
    Texture2D texture = LoadTexture(path.c_str());
    if (texture.id == 0) {
        // Create a simple colored rectangle as fallback
        Image img = GenImageColor(FALLBACK_TEXTURE_SIZE, FALLBACK_TEXTURE_SIZE, RED);
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
        consoleCapture.addLine("Using fallback texture - failed to load: " + path);
    } else {
        consoleCapture.addLine("Player texture loaded successfully (" + path + ")");
    }
    return texture;
}