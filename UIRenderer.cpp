#include "UIRenderer.h"
#include "ConsoleCapture.h"
#include "raylib.h"
#include <algorithm>

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
    constexpr const char* quitText = "Press Q to quit";
    
    // Calculate centered text positions
    const int titleWidth = MeasureText(titleText, titleFontSize);
    const int titleX = pauseBoxX + (PAUSE_BOX_WIDTH - titleWidth) / 2;
    
    const int resumeWidth = MeasureText(resumeText, instructionFontSize);
    const int resumeX = pauseBoxX + (PAUSE_BOX_WIDTH - resumeWidth) / 2;
    
    const int quitWidth = MeasureText(quitText, instructionFontSize);
    const int quitX = pauseBoxX + (PAUSE_BOX_WIDTH - quitWidth) / 2;
    
    // Draw centered text
    DrawText(titleText, titleX, pauseBoxY + 30, titleFontSize, WHITE);
    DrawText(resumeText, resumeX, pauseBoxY + 70, instructionFontSize, LIGHTGRAY);
    DrawText(quitText, quitX, pauseBoxY + 95, instructionFontSize, LIGHTGRAY);
}