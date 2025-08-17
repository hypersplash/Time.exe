#include "ConsoleCapture.h"

// Global console capture instance
ConsoleCapture consoleCapture;

ConsoleCapture::ConsoleCapture() {
    // deque doesn't need reserve() - it manages memory efficiently
}

void ConsoleCapture::setMaxDisplayChars(int maxChars) {
    maxDisplayChars = maxChars;
}

void ConsoleCapture::addLine(const std::string& line) {
    std::string truncated = line;
    if (truncated.length() > static_cast<size_t>(maxDisplayChars)) {
        truncated = truncated.substr(0, maxDisplayChars - 3) + "...";
    }
    lines.push_back(truncated);
    if (lines.size() > MAX_LINES) {
        lines.pop_front();
    }
}

const std::deque<std::string>& ConsoleCapture::getLines() const {
    return lines;
}

void ConsoleCapture::clear() {
    lines.clear();
}