#ifndef VERSION_H
#define VERSION_H

// Game identity
constexpr const char* GAME_NAME = "Abandoned Where?";
constexpr const char* GAME_NAME_CAPS = "ABANDONED WHERE?";
constexpr const char* GAME_NAME_SHORT = "AbandonedWhere";

// Version information
constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 1;
constexpr int VERSION_PATCH = 10;
constexpr const char* VERSION_STAGE = "ALPHA";

// Pre-formatted version strings for convenience
constexpr const char* VERSION_SHORT = "v0.1.10 ALPHA";
constexpr const char* VERSION_FULL = "Abandoned Where? v0.1.10 ALPHA";

// Window titles for different contexts
constexpr const char* WINDOW_TITLE = "Abandoned Where?";
constexpr const char* WINDOW_TITLE_DEBUG = "Abandoned Where? (DEBUG)";
constexpr const char* WINDOW_TITLE_TESTING = "Abandoned Where? (TESTING MODE)";

#endif // VERSION_H