#include "UIRenderer.h"
#include "ConsoleCapture.h"
#include "raylib.h"
#include <algorithm>
#include <cmath>

// Global console layout cache
static ConsoleLayout consoleLayout;

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
    
    // Cache current parameters
    lastShowFPS = showFPS;
    lastWidth = consoleWidth;
    lastHeight = consoleHeight;
    lastFontSize = consoleFontSize;
    needsUpdate = false;
}

bool ConsoleLayout::shouldUpdate(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) const {
    return needsUpdate || 
           lastShowFPS != showFPS || 
           lastWidth != consoleWidth || 
           lastHeight != consoleHeight || 
           lastFontSize != consoleFontSize;
}

void DrawConsole(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) {
    // Only recalculate layout when parameters change
    if (consoleLayout.shouldUpdate(showFPS, consoleWidth, consoleHeight, consoleFontSize)) {
        consoleLayout.update(showFPS, consoleWidth, consoleHeight, consoleFontSize);
    }
    
    const auto& layout = consoleLayout;
    
    // Draw console background and border
    DrawRectangle(layout.x, layout.y, layout.width, layout.height, 
                  Fade(BLACK, CONSOLE_BACKGROUND_ALPHA));
    DrawRectangleLines(layout.x, layout.y, layout.width, layout.height, WHITE);
    
    // Draw console title
    DrawText("Console Output:", 
             layout.x + layout.padding, 
             layout.y + layout.padding, 
             layout.titleFontSize, WHITE);
    
    // Draw console lines
    const auto& lines = consoleCapture.getLines();
    if (lines.empty()) return;  // Early exit if no lines
    
    const int startY = layout.y + layout.titleHeight;
    const int linesToShow = std::min(static_cast<int>(lines.size()), layout.maxDisplayLines);
    const int startIndex = std::max(0, static_cast<int>(lines.size()) - linesToShow);
    
    // Render lines efficiently
    for (int i = 0; i < linesToShow; ++i) {
        const int lineIndex = startIndex + i;
        const int textY = startY + (i * layout.lineHeight);
        
        DrawText(lines[lineIndex].c_str(), 
                 layout.x + layout.padding, 
                 textY, 
                 layout.textFontSize, LIGHTGRAY);
    }
}

void DrawTitleScreen(int screenWidth, int screenHeight) {
    // Clear background with a slightly different color for title screen
    ClearBackground(DARKGRAY);
    
    // Title text configuration
    constexpr const char* gameTitle = "ABANDONED WHERE";
    constexpr const char* startInstruction = "Press SPACE to start";
    constexpr const char* quitInstruction = "Press ESC to quit";
    
    // Calculate font sizes based on screen size for better scaling
    const int titleFontSize = std::max(48, screenWidth / 20);
    const int instructionFontSize = std::max(20, screenWidth / 40);
    
    // Calculate title position (centered)
    const int titleWidth = MeasureText(gameTitle, titleFontSize);
    const int titleX = (screenWidth - titleWidth) / 2;
    const int titleY = screenHeight / 2 - 100;
    
    // Calculate instruction positions (centered)
    const int startWidth = MeasureText(startInstruction, instructionFontSize);
    const int startX = (screenWidth - startWidth) / 2;
    const int startY = titleY + titleFontSize + 60;
    
    const int quitWidth = MeasureText(quitInstruction, instructionFontSize);
    const int quitX = (screenWidth - quitWidth) / 2;
    const int quitY = startY + instructionFontSize + 20;
    
    // Draw title with a subtle glow effect
    // Shadow/glow effect (draw slightly offset in darker color)
    DrawText(gameTitle, titleX + 3, titleY + 3, titleFontSize, BLACK);
    DrawText(gameTitle, titleX, titleY, titleFontSize, WHITE);
    
    // Draw instructions with a softer color
    DrawText(startInstruction, startX, startY, instructionFontSize, LIGHTGRAY);
    DrawText(quitInstruction, quitX, quitY, instructionFontSize, GRAY);
    
    // Add a simple animated element - pulsing "Press SPACE" text
    static float pulseTime = 0.0f;
    pulseTime += GetFrameTime() * 3.0f;  // Speed of pulse
    const float pulse = (sin(pulseTime) + 1.0f) / 2.0f;  // 0.0 to 1.0
    const Color pulseColor = {
        static_cast<unsigned char>(LIGHTGRAY.r),
        static_cast<unsigned char>(LIGHTGRAY.g), 
        static_cast<unsigned char>(LIGHTGRAY.b),
        static_cast<unsigned char>(255 * (0.5f + pulse * 0.5f))  // Alpha from 128 to 255
    };
    
    // Redraw start instruction with pulsing effect
    DrawText(startInstruction, startX, startY, instructionFontSize, pulseColor);
    
    // Add a simple version text in the corner
    constexpr const char* versionText = "v0.1 - Development Build";
    constexpr int versionFontSize = 16;
    const int versionY = screenHeight - versionFontSize - 10;
    DrawText(versionText, 10, versionY, versionFontSize, DARKGRAY);
}

void DrawPauseScreen(int screenWidth, int screenHeight) {
    // Semi-transparent overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, PAUSE_OVERLAY_ALPHA));
    
    // Calculate centered pause box position
    const int pauseBoxX = (screenWidth - PAUSE_BOX_WIDTH) / 2;
    const int pauseBoxY = (screenHeight - PAUSE_BOX_HEIGHT) / 2;
    
    // Draw pause menu background and border
    DrawRectangle(pauseBoxX, pauseBoxY, PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT, BLACK);
    DrawRectangleLines(pauseBoxX, pauseBoxY, PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT, WHITE);
    
    // Text configuration
    constexpr int titleFontSize = 24;
    constexpr int instructionFontSize = 16;
    constexpr const char* titleText = "GAME PAUSED";
    constexpr const char* resumeText = "Press ESC to resume";
    constexpr const char* titleText2 = "Press T to return to title";
    constexpr const char* quitText = "Press Q to quit";
    
    // Calculate centered text positions
    const int titleWidth = MeasureText(titleText, titleFontSize);
    const int titleX = pauseBoxX + (PAUSE_BOX_WIDTH - titleWidth) / 2;
    
    const int resumeWidth = MeasureText(resumeText, instructionFontSize);
    const int resumeX = pauseBoxX + (PAUSE_BOX_WIDTH - resumeWidth) / 2;
    
    const int titleWidth2 = MeasureText(titleText2, instructionFontSize);
    const int titleX2 = pauseBoxX + (PAUSE_BOX_WIDTH - titleWidth2) / 2;
    
    const int quitWidth = MeasureText(quitText, instructionFontSize);
    const int quitX = pauseBoxX + (PAUSE_BOX_WIDTH - quitWidth) / 2;
    
    // Draw centered text with better spacing
    DrawText(titleText, titleX, pauseBoxY + 20, titleFontSize, WHITE);
    DrawText(resumeText, resumeX, pauseBoxY + 55, instructionFontSize, LIGHTGRAY);
    DrawText(titleText2, titleX2, pauseBoxY + 80, instructionFontSize, LIGHTGRAY);
    DrawText(quitText, quitX, pauseBoxY + 105, instructionFontSize, LIGHTGRAY);
}