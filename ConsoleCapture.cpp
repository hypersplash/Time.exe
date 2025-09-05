#include "ConsoleCapture.h"

// Global console capture instance initialization.
ConsoleCapture consoleCapture;

// Sets the maximum number of characters to display for each line in the console.
void ConsoleCapture::setMaxDisplayChars(int maxChars) {
    maxDisplayChars = maxChars;
}

// Adds a line to the console, truncating it if it's too long.
void ConsoleCapture::addLine(const std::string& line) {
    // If the line is longer than the maximum displayable characters, truncate it.
    if (line.length() > static_cast<size_t>(maxDisplayChars)) {
        addTruncatedLine(line);
    } else {
        // Otherwise, add the line as is.
        lines.emplace_back(line);
    }
    
    // If the number of lines exceeds the maximum, remove the oldest line.
    if (lines.size() > MAX_LINES) {
        lines.pop_front();
    }
}

// Helper function to add a truncated line to the console.
void ConsoleCapture::addTruncatedLine(const std::string& line) {
    // Reserve memory for the truncated string to avoid re-allocations.
    std::string truncated;
    truncated.reserve(maxDisplayChars);
    // Truncate the line and add an ellipsis.
    truncated = line.substr(0, maxDisplayChars - 3) + "...";
    // Add the truncated line to the console.
    lines.emplace_back(std::move(truncated));
}