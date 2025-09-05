#ifndef UI_RENDERER_H
#define UI_RENDERER_H
#include <string>

// Constants for UI rendering.
constexpr float PAUSE_OVERLAY_ALPHA = 0.6f;      // The alpha (transparency) of the pause screen overlay.
constexpr float CONSOLE_BACKGROUND_ALPHA = 0.8f; // The alpha of the console background.
constexpr int PAUSE_BOX_WIDTH = 300;             // The width of the pause menu box.
constexpr int PAUSE_BOX_HEIGHT = 170;            // The height of the pause menu box.
constexpr int CONSOLE_LINE_SPACING = 4;          // The vertical spacing between lines in the console.

// The ConsoleLayout struct holds the cached layout information for the console.
// This is used to avoid recalculating the layout every frame.
struct ConsoleLayout {
    int x, y, width, height; // The position and dimensions of the console.
    int titleFontSize, textFontSize; // The font sizes for the title and text.
    int lineHeight, padding; // The height of each line and the padding around the text.
    int titleHeight, availableHeight; // The height of the title area and the available height for lines.
    int maxDisplayLines; // The maximum number of lines that can be displayed.
    bool needsUpdate = true; // Whether the layout needs to be recalculated.
    
    // Updates the console layout.
    void update(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize);
    // Checks if the console layout needs to be updated.
    bool shouldUpdate(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) const;
    
private:
    // The last known parameters, used to detect changes.
    bool lastShowFPS = false;
    int lastWidth = 0, lastHeight = 0, lastFontSize = 0;
};

// The ConsoleInput struct holds the state of the console input box.
struct ConsoleInput {
    std::string text; // The text currently in the input box.
    int cursorPosition = 0; // The position of the cursor in the text.
    bool active = false; // Whether the input box is active.
};

// Renders the developer console.
void DrawConsole(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize, const ConsoleInput& consoleInput);

// Renders the console input box.
void DrawConsoleInputBox(const ConsoleLayout& layout, const ConsoleInput& consoleInput);

// Renders the title screen.
void DrawTitleScreen(int screenWidth, int screenHeight);

// Renders the pause screen.
void DrawPauseScreen(int screenWidth, int screenHeight);

#endif // UI_RENDERER_H