#include "ConsoleCapture.h"

// Global console capture instance
ConsoleCapture consoleCapture;

void ConsoleCapture::setMaxDisplayChars(int maxChars) {
    maxDisplayChars = maxChars;
}

void ConsoleCapture::addLine(const std::string& line) {
    // Only truncate if necessary - avoid unnecessary string operations
    if (line.length() > static_cast<size_t>(maxDisplayChars)) {
        addTruncatedLine(line);
    } else {
        lines.emplace_back(line);  // Direct construction, no extra copies
    }
    
    // Maintain maximum line count
    if (lines.size() > MAX_LINES) {
        lines.pop_front();
    }
}

void ConsoleCapture::addTruncatedLine(const std::string& line) {
    // Reserve space to avoid reallocation during concatenation
    std::string truncated;
    truncated.reserve(maxDisplayChars);
    truncated = line.substr(0, maxDisplayChars - 3) + "...";
    lines.emplace_back(std::move(truncated));
}