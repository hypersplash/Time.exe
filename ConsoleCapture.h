#ifndef CONSOLE_CAPTURE_H
#define CONSOLE_CAPTURE_H

#include <string>
#include <deque>

// The ConsoleCapture class is responsible for capturing and managing console output lines.
// It stores a limited number of lines and provides methods to add and retrieve them.
class ConsoleCapture {
private:
    // A deque to store the console lines.
    std::deque<std::string> lines;
    // The maximum number of lines to store.
    static const int MAX_LINES = 15;
    // The maximum number of characters to display per line.
    int maxDisplayChars = 50;
    
public:
    // Default constructor.
    ConsoleCapture() = default;
    
    // Sets the maximum number of displayable characters for each line.
    void setMaxDisplayChars(int maxChars);
    // Adds a new line to the console.
    // If the line is longer than maxDisplayChars, it will be truncated.
    void addLine(const std::string& line);
    // Returns a constant reference to the deque of lines.
    const std::deque<std::string>& getLines() const { return lines; }
    // Clears all lines from the console.
    void clear() { lines.clear(); }
    
private:
    // A helper function to add a truncated line to the deque.
    void addTruncatedLine(const std::string& line);
};

// A global instance of the ConsoleCapture class that can be accessed from anywhere in the application.
extern ConsoleCapture consoleCapture;

#endif // CONSOLE_CAPTURE_H