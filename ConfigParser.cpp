#include "ConfigParser.h"
#include "ConsoleCapture.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace {
    // Helper function to trim whitespace from both ends of a string.
    void trimWhitespace(std::string& str) {
        // Trim leading whitespace.
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        
        // Trim trailing whitespace.
        str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), str.end());
    }
    
    // Helper function to trim quotes from the beginning and end of a string.
    void trimQuotes(std::string& str) {
        if (!str.empty() && (str.front() == '"' || str.front() == '\'')) {
            str.erase(0, 1);
        }
        if (!str.empty() && (str.back() == '"' || str.back() == '\'')) {
            str.pop_back();
        }
    }
    
    // Checks if a line is a comment or empty and should be skipped.
    bool shouldSkipLine(const std::string& line) {
        return line.empty() || line[0] == '#';
    }
}

// Parses an INI file and returns a map of key-value pairs.
std::unordered_map<std::string, std::string> parseINI(const std::string& filepath) {
    std::unordered_map<std::string, std::string> result;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        const std::string errorMsg = "Failed to open " + filepath;
        std::cerr << errorMsg << '\n';
        consoleCapture.addLine(errorMsg);
        return result;
    }
    
    std::string line;
    line.reserve(256);  // Reserve space to reduce re-allocations in the loop.
    
    // Read the file line by line.
    while (std::getline(file, line)) {
        trimWhitespace(line);
        
        if (shouldSkipLine(line)) {
            continue;
        }
        
        // Find the position of the '=' separator.
        const size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) {
            continue;  // Skip lines that do not contain a '='.
        }
        
        // Extract the key and value from the line.
        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);
        
        // Clean up the key and value.
        trimWhitespace(key);
        trimWhitespace(value);
        trimQuotes(value);
        
        // Add the key-value pair to the map if the key is not empty.
        if (!key.empty()) {
            result.emplace(std::move(key), std::move(value));
        }
    }
    
    return result;
}
