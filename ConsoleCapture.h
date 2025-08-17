#ifndef CONSOLE_CAPTURE_H
#define CONSOLE_CAPTURE_H

#include <string>
#include <deque>

class ConsoleCapture {
private:
    std::deque<std::string> lines;
    static const int MAX_LINES = 15;
    int maxDisplayChars = 50;
    
public:
    ConsoleCapture() = default;
    
    void setMaxDisplayChars(int maxChars);
    void addLine(const std::string& line);
    const std::deque<std::string>& getLines() const { return lines; }
    void clear() { lines.clear(); }
    
private:
    // Helper to avoid redundant string operations
    void addTruncatedLine(const std::string& line);
};

// Global console capture instance
extern ConsoleCapture consoleCapture;

#endif // CONSOLE_CAPTURE_H