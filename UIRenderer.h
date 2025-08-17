#ifndef UI_RENDERER_H
#define UI_RENDERER_H

// Constants for UI rendering
constexpr float PAUSE_OVERLAY_ALPHA = 0.6f;
constexpr float CONSOLE_BACKGROUND_ALPHA = 0.8f;
constexpr int PAUSE_BOX_WIDTH = 300;
constexpr int PAUSE_BOX_HEIGHT = 170;
constexpr int CONSOLE_LINE_SPACING = 4;

// Cached console layout for performance
struct ConsoleLayout {
    int x, y, width, height;
    int titleFontSize, textFontSize;
    int lineHeight, padding;
    int titleHeight, availableHeight;
    int maxDisplayLines;
    bool needsUpdate = true;
    
    void update(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize);
    bool shouldUpdate(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) const;
    
private:
    bool lastShowFPS = false;
    int lastWidth = 0, lastHeight = 0, lastFontSize = 0;
};

// Console rendering function with cached layout
void DrawConsole(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize);

// Title screen rendering function
void DrawTitleScreen(int screenWidth, int screenHeight);

// Pause screen rendering function
void DrawPauseScreen(int screenWidth, int screenHeight);

#endif // UI_RENDERER_H