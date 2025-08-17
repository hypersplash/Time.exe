#ifndef CONSOLE_CAPTURE_H
#define CONSOLE_CAPTURE_H

#include <string>
#include <deque>

class ConsoleCapture {
private:
    std::deque<std::string> lines;
    static const int MAX_LINES = 15;
    int maxDisplayChars = 50; // Will be updated based on console width
    
public:
    ConsoleCapture();
    
    void setMaxDisplayChars(int maxChars);
    void addLine(const std::string& line);
    const std::deque<std::string>& getLines() const;
    void clear();
};

// Global console capture instance
extern ConsoleCapture consoleCapture;

#endif // CONSOLE_CAPTURE_H