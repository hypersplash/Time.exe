#include "UIRenderer.h"
#include "ConsoleCapture.h"
#include "Version.h"
#include "raylib.h"
#include <algorithm>
#include <cmath>

// A global cache for the console layout to avoid recalculating it every frame.
static ConsoleLayout consoleLayout;

// Updates the console layout cache with the current parameters.
void ConsoleLayout::update(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) {
    x = 10;
    y = showFPS ? 40 : 10;
    width = consoleWidth;
    height = consoleHeight;
    
    titleFontSize = consoleFontSize + 2;
    textFontSize = consoleFontSize;
    lineHeight = textFontSize + CONSOLE_LINE_SPACING;
    padding = consoleFontSize / 2;
    
    titleHeight = titleFontSize + padding;
    availableHeight = height - titleHeight - padding;
    maxDisplayLines = std::max(1, availableHeight / lineHeight);
    
    // Cache the current parameters to detect changes.
    lastShowFPS = showFPS;
    lastWidth = consoleWidth;
    lastHeight = consoleHeight;
    lastFontSize = consoleFontSize;
    needsUpdate = false;
}

// Checks if the console layout needs to be updated based on the current parameters.
bool ConsoleLayout::shouldUpdate(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) const {
    return needsUpdate || 
           lastShowFPS != showFPS || 
           lastWidth != consoleWidth || 
           lastHeight != consoleHeight || 
           lastFontSize != consoleFontSize;
}

// Draws the developer console on the screen.
void DrawConsole(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize, const ConsoleInput& consoleInput) {
    // Recalculate the console layout only when necessary.
    if (consoleLayout.shouldUpdate(showFPS, consoleWidth, consoleHeight, consoleFontSize)) {
        consoleLayout.update(showFPS, consoleWidth, consoleHeight, consoleFontSize);
    }
    
    const auto& layout = consoleLayout;
    
    // Draw the console's background and border.
    DrawRectangle(layout.x, layout.y, layout.width, layout.height, 
                  Fade(BLACK, CONSOLE_BACKGROUND_ALPHA));
    DrawRectangleLines(layout.x, layout.y, layout.width, layout.height, WHITE);
    
    // Draw the console title.
    DrawText("Console Output:", 
             layout.x + layout.padding, 
             layout.y + layout.padding, 
             layout.titleFontSize, WHITE);
    
    // Draw the captured console lines.
    const auto& lines = consoleCapture.getLines();
    if (lines.empty()) return;  // No lines to draw.
    
    const int startY = layout.y + layout.titleHeight;
    const int linesToShow = std::min(static_cast<int>(lines.size()), layout.maxDisplayLines);
    const int startIndex = std::max(0, static_cast<int>(lines.size()) - linesToShow);
    
    // Render the visible lines efficiently.
    for (int i = 0; i < linesToShow; ++i) {
        const int lineIndex = startIndex + i;
        const int textY = startY + (i * layout.lineHeight);
        
        DrawText(lines[lineIndex].c_str(), 
                 layout.x + layout.padding, 
                 textY, 
                 layout.textFontSize, LIGHTGRAY);
    }
    // Draw the console input box.
    DrawConsoleInputBox(layout, consoleInput);
}

// Draws the console input box at the bottom of the console.
void DrawConsoleInputBox(const ConsoleLayout& layout, const ConsoleInput& consoleInput) {
    const int inputBoxHeight = layout.textFontSize + layout.padding * 2;
    const int inputBoxY = layout.y + layout.height - inputBoxHeight;

    // Draw the input box background and border.
    DrawRectangle(layout.x, inputBoxY, layout.width, inputBoxHeight, Fade(BLACK, 0.5f));
    DrawRectangleLines(layout.x, inputBoxY, layout.width, inputBoxHeight, WHITE);

    const int textX = layout.x + layout.padding;
    const int textY = inputBoxY + layout.padding;

    // Draw the input text.
    DrawText(consoleInput.text.c_str(), textX, textY, layout.textFontSize, WHITE);

    // Draw the cursor if the input box is active.
    if (consoleInput.active) {
        const int cursorX = textX + MeasureText(consoleInput.text.substr(0, consoleInput.cursorPosition).c_str(), layout.textFontSize);
        DrawLine(cursorX, textY, cursorX, textY + layout.textFontSize, WHITE);
    }
}

// Draws the title screen.
void DrawTitleScreen(int screenWidth, int screenHeight) {
    ClearBackground(DARKGRAY);
    
    // Title text configuration.
    constexpr const char* gameTitle = GAME_NAME_CAPS;
    constexpr const char* startInstruction = "Press SPACE to start";
    constexpr const char* quitInstruction = "Press ESC to quit";
    
    // Scale font sizes based on screen dimensions for better readability.
    const int titleFontSize = std::max(48, screenWidth / 20);
    const int instructionFontSize = std::max(20, screenWidth / 40);
    
    // Calculate centered positions for the text.
    const int titleWidth = MeasureText(gameTitle, titleFontSize);
    const int titleX = (screenWidth - titleWidth) / 2;
    const int titleY = screenHeight / 2 - 100;
    
    const int startWidth = MeasureText(startInstruction, instructionFontSize);
    const int startX = (screenWidth - startWidth) / 2;
    const int startY = titleY + titleFontSize + 60;
    
    const int quitWidth = MeasureText(quitInstruction, instructionFontSize);
    const int quitX = (screenWidth - quitWidth) / 2;
    const int quitY = startY + instructionFontSize + 20;
    
    // Draw the title with a subtle shadow/glow effect.
    DrawText(gameTitle, titleX + 3, titleY + 3, titleFontSize, BLACK);
    DrawText(gameTitle, titleX, titleY, titleFontSize, WHITE);
    
    // Draw the instructions.
    DrawText(startInstruction, startX, startY, instructionFontSize, LIGHTGRAY);
    DrawText(quitInstruction, quitX, quitY, instructionFontSize, GRAY);
    
    // Create a simple pulsing animation for the "Press SPACE" text.
    static float pulseTime = 0.0f;
    pulseTime += GetFrameTime() * 3.0f;
    const float pulse = (sin(pulseTime) + 1.0f) / 2.0f;
    const Color pulseColor = {
        static_cast<unsigned char>(LIGHTGRAY.r),
        static_cast<unsigned char>(LIGHTGRAY.g), 
        static_cast<unsigned char>(LIGHTGRAY.b),
        static_cast<unsigned char>(255 * (0.5f + pulse * 0.5f))
    };
    
    // Redraw the start instruction with the pulsing color.
    DrawText(startInstruction, startX, startY, instructionFontSize, pulseColor);
    
    // Draw the game version in the bottom right corner.
    constexpr int versionFontSize = 16;
    const int versionWidth = MeasureText(VERSION_SHORT, versionFontSize);
    const int versionX = screenWidth - versionWidth - 10;
    const int versionY = screenHeight - versionFontSize - 10;
    DrawText(VERSION_SHORT, versionX, versionY, versionFontSize, BLACK);
}

// Draws the pause screen overlay.
void DrawPauseScreen(int screenWidth, int screenHeight) {
    // Draw a semi-transparent overlay to dim the background.
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, PAUSE_OVERLAY_ALPHA));
    
    // Calculate the position for the centered pause box.
    const int pauseBoxX = (screenWidth - PAUSE_BOX_WIDTH) / 2;
    const int pauseBoxY = (screenHeight - PAUSE_BOX_HEIGHT) / 2;
    
    // Draw the pause menu background and border.
    DrawRectangle(pauseBoxX, pauseBoxY, PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT, BLACK);
    DrawRectangleLines(pauseBoxX, pauseBoxY, PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT, WHITE);
    
    // Text configuration for the pause menu.
    constexpr int titleFontSize = 24;
    constexpr int instructionFontSize = 16;
    constexpr const char* titleText = "GAME PAUSED";
    constexpr const char* resumeText = "Press ESC to resume";
    constexpr const char* titleText2 = "Press T to return to title";
    constexpr const char* quitText = "Press Q to quit";
    
    // Calculate centered text positions within the pause box.
    const int titleWidth = MeasureText(titleText, titleFontSize);
    const int titleX = pauseBoxX + (PAUSE_BOX_WIDTH - titleWidth) / 2;
    
    const int resumeWidth = MeasureText(resumeText, instructionFontSize);
    const int resumeX = pauseBoxX + (PAUSE_BOX_WIDTH - resumeWidth) / 2;
    
    const int titleWidth2 = MeasureText(titleText2, instructionFontSize);
    const int titleX2 = pauseBoxX + (PAUSE_BOX_WIDTH - titleWidth2) / 2;
    
    const int quitWidth = MeasureText(quitText, instructionFontSize);
    const int quitX = pauseBoxX + (PAUSE_BOX_WIDTH - quitWidth) / 2;
    
    // Draw the text in the pause menu.
    DrawText(titleText, titleX, pauseBoxY + 20, titleFontSize, WHITE);
    DrawText(resumeText, resumeX, pauseBoxY + 55, instructionFontSize, LIGHTGRAY);
    DrawText(titleText2, titleX2, pauseBoxY + 80, instructionFontSize, LIGHTGRAY);
    DrawText(quitText, quitX, pauseBoxY + 105, instructionFontSize, LIGHTGRAY);
}