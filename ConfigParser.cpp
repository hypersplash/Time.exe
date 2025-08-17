#include "ConfigParser.h"
#include "ConsoleCapture.h"
#include <fstream>
#include <iostream>
#include <algorithm>

namespace {
    // Helper function to trim whitespace (more efficient than repeated find operations)
    void trimWhitespace(std::string& str) {
        // Trim leading whitespace
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        
        // Trim trailing whitespace
        str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), str.end());
    }
    
    // Helper to trim quotes from config values
    void trimQuotes(std::string& str) {
        if (!str.empty() && (str.front() == '"' || str.front() == '\'')) {
            str.erase(0, 1);
        }
        if (!str.empty() && (str.back() == '"' || str.back() == '\'')) {
            str.pop_back();
        }
    }
    
    // Check if line should be skipped (empty or comment)
    bool shouldSkipLine(const std::string& line) {
        return line.empty() || line[0] == '#';
    }
}

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
    line.reserve(256);  // Reserve space to reduce allocations
    
    while (std::getline(file, line)) {
        trimWhitespace(line);
        
        if (shouldSkipLine(line)) {
            continue;
        }
        
        // Find the key-value separator
        const size_t equalPos = line.find('=');
        if (equalPos == std::string::npos) {
            continue;  // Skip malformed lines
        }
        
        // Extract key and value
        std::string key = line.substr(0, equalPos);
        std::string value = line.substr(equalPos + 1);
        
        // Trim and clean key and value
        trimWhitespace(key);
        trimWhitespace(value);
        trimQuotes(value);
        
        // Only add non-empty keys
        if (!key.empty()) {
            result.emplace(std::move(key), std::move(value));
        }
    }
    
    return result;
}