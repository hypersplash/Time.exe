#include "UIRenderer.h"
#include "ConsoleCapture.h"
#include "raylib.h"
#include <algorithm>

void DrawConsole(bool showFPS, int consoleWidth, int consoleHeight, int consoleFontSize) {
    // Console box dimensions (cached calculations)
    static int consoleX = 10;
    static int consoleY = showFPS ? 40 : 10;
    static int titleFontSize = consoleFontSize + 2;
    static int textFontSize = consoleFontSize;
    static int lineHeight = textFontSize + CONSOLE_LINE_SPACING;
    static int padding = consoleFontSize / 2;
    
    // Update consoleY if showFPS changed
    consoleY = showFPS ? 40 : 10;
    
    // Draw console background
    DrawRectangle(consoleX, consoleY, consoleWidth, consoleHeight, Fade(BLACK, CONSOLE_BACKGROUND_ALPHA));
    DrawRectangleLines(consoleX, consoleY, consoleWidth, consoleHeight, WHITE);
    
    // Draw console title
    DrawText("Console Output:", consoleX + padding, consoleY + padding, titleFontSize, WHITE);
    
    // Calculate available space for text lines
    int titleHeight = titleFontSize + padding;
    int availableHeight = consoleHeight - titleHeight - padding;
    int maxDisplayLines = availableHeight / lineHeight;
    
    // Draw console lines
    const auto& lines = consoleCapture.getLines();
    int startY = consoleY + titleHeight;
    
    // Calculate how many lines to show
    int linesToShow = std::min(static_cast<int>(lines.size()), maxDisplayLines);
    int startIndex = std::max(0, static_cast<int>(lines.size()) - linesToShow);
    
    // Loop through each line we can display
    for (int i = 0; i < linesToShow; i++) {
        int lineIndex = startIndex + i;
        int textY = startY + (i * lineHeight);
        
        DrawText(lines[lineIndex].c_str(), consoleX + padding, textY, textFontSize, LIGHTGRAY);
    }
}

void DrawPauseScreen(int screenWidth, int screenHeight) {
    // Semi-transparent overlay
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, PAUSE_OVERLAY_ALPHA));
    
    // Pause menu box - centered using the formula
    int pauseBoxX = (screenWidth - PAUSE_BOX_WIDTH) / 2;
    int pauseBoxY = (screenHeight - PAUSE_BOX_HEIGHT) / 2;
    
    // Draw pause menu background
    DrawRectangle(pauseBoxX, pauseBoxY, PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT, BLACK);
    DrawRectangleLines(pauseBoxX, pauseBoxY, PAUSE_BOX_WIDTH, PAUSE_BOX_HEIGHT, WHITE);
    
    // Draw pause text - centered within the pause box
    const int titleFontSize = 24;
    const int instructionFontSize = 16;
    
    // Calculate text centering within the pause box
    const char* titleText = "GAME PAUSED";
    const char* resumeText = "Press ESC to resume";
    const char* quitText = "Press Q to quit";
    
    // Center the title text horizontally within the pause box
    int titleWidth = MeasureText(titleText, titleFontSize);
    int titleX = pauseBoxX + (PAUSE_BOX_WIDTH - titleWidth) / 2;
    
    // Center the instruction texts horizontally within the pause box
    int resumeWidth = MeasureText(resumeText, instructionFontSize);
    int resumeX = pauseBoxX + (PAUSE_BOX_WIDTH - resumeWidth) / 2;
    
    int quitWidth = MeasureText(quitText, instructionFontSize);
    int quitX = pauseBoxX + (PAUSE_BOX_WIDTH - quitWidth) / 2;
    
    // Draw centered text
    DrawText(titleText, titleX, pauseBoxY + 30, titleFontSize, WHITE);
    DrawText(resumeText, resumeX, pauseBoxY + 70, instructionFontSize, LIGHTGRAY);
    DrawText(quitText, quitX, pauseBoxY + 95, instructionFontSize, LIGHTGRAY);
}