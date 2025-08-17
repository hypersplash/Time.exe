#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <unordered_map>
#include <string>

// Simple INI parser
std::unordered_map<std::string, std::string> parseINI(const std::string& filepath);

#endif // CONFIG_PARSER_H