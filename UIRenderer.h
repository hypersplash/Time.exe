#ifndef UI_RENDERER_H
#define UI_RENDERER_H

// Constants for UI rendering
const float PAUSE_OVERLAY_ALPHA = 0.6f;
const float CONSOLE_BACKGROUND_ALPHA = 0.8f;
const int PAUSE_BOX_WIDTH = 300;
const int PAUSE_BOX_HEIGHT = 150;
const int CONSOLE_LINE_SPACING = 4;

// Console rendering function
void DrawConsole(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize);

// Pause screen rendering function
void DrawPauseScreen(int screenWidth, int screenHeight);

#endif // UI_RENDERER_H