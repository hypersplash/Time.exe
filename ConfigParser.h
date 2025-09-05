#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <unordered_map>
#include <string>

// Declares the function to parse an INI file.
// The function takes a file path as input and returns a map of key-value pairs.
std::unordered_map<std::string, std::string> parseINI(const std::string& filepath);

#endif // CONFIG_PARSER_H