#ifndef VERSION_H
#define VERSION_H

// Game identity constants.
constexpr const char* GAME_NAME = "Time.exe";
constexpr const char* GAME_NAME_CAPS = "TIME.EXE";
constexpr const char* GAME_NAME_SHORT = "TimeExe";

// Version information.
constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 1;
constexpr int VERSION_PATCH = 10;
constexpr const char* VERSION_STAGE = "ALPHA";

// Pre-formatted version strings for convenience.
constexpr const char* VERSION_SHORT = "v0.1.10 ALPHA";
constexpr const char* VERSION_FULL = "Time.exe - v0.1.10 ALPHA";

// Window titles for different build configurations.
constexpr const char* WINDOW_TITLE = "Time.exe";
constexpr const char* WINDOW_TITLE_DEBUG = "Time.exe (DEBUG)";
constexpr const char* WINDOW_TITLE_TESTING = "Time.exe (TESTING MODE)";

#endif // VERSION_H