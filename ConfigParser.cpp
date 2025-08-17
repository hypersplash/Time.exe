#include "ConfigParser.h"
#include "ConsoleCapture.h"
#include <fstream>
#include <iostream>

std::unordered_map<std::string, std::string> parseINI(const std::string& filepath) {
    std::unordered_map<std::string, std::string> result;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::string errorMsg = "Failed to open " + filepath;
        std::cerr << errorMsg << "\n";
        consoleCapture.addLine(errorMsg);
        return result;
    }
    std::string line;
    while (std::getline(file, line)) {
        // Remove whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;
        
        // Split at '='
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        
        // Trim whitespace and quotes
        key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        value.erase(0, value.find_first_not_of(" \t\r\n\""));
        value.erase(value.find_last_not_of(" \t\r\n\"") + 1);
        
        result[key] = value;
    }
    return result;
}