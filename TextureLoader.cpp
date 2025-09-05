#include "TextureLoader.h"
#include "ConsoleCapture.h"

// Loads the player texture from the given path.
// If the texture fails to load, it creates a fallback texture.
Texture2D LoadPlayerTexture(const std::string& path) {
    // Attempt to load the texture from the specified path.
    Texture2D texture = LoadTexture(path.c_str());
    // Check if the texture was loaded successfully.
    if (texture.id == 0) {
        // If loading fails, create a fallback texture (a simple red square).
        Image img = GenImageColor(FALLBACK_TEXTURE_SIZE, FALLBACK_TEXTURE_SIZE, RED);
        texture = LoadTextureFromImage(img);
        UnloadImage(img);
        // Log a message to the console indicating that the fallback texture is being used.
        consoleCapture.addLine("Using fallback texture - failed to load: " + path);
    } else {
        // Log a success message to the console.
        consoleCapture.addLine("Player texture loaded successfully (" + path + ")");
    }
    return texture;
}