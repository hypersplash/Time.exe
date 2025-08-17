# ============================================================================
# PROJECT CONFIGURATION
# ============================================================================
TARGET = abadonedwhere
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# ============================================================================
# DIRECTORIES
# ============================================================================
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = .

# ============================================================================
# RAYLIB CONFIGURATION
# ============================================================================
# Override with: make RAYLIB_PREFIX=/path/to/raylib/install
RAYLIB_PREFIX ?= /usr/local
RAYLIB_INC = $(RAYLIB_PREFIX)/include
RAYLIB_LIBDIR = $(RAYLIB_PREFIX)/lib
RAYLIB_A = $(RAYLIB_LIBDIR)/libraylib.a

# System libraries for raylib
LIBS = -lGL -lm -lpthread -ldl -lrt -lX11

# ============================================================================
# SOURCE FILES
# ============================================================================
SOURCES = main.cpp \
          ConsoleCapture.cpp \
          ConfigParser.cpp \
          TextureLoader.cpp \
          UIRenderer.cpp \
          Player.cpp \
          GameConfig.cpp

OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/$(TARGET)

# ============================================================================
# BUILD RULES
# ============================================================================
.PHONY: all clean rebuild help

# Default target
all: $(EXECUTABLE)

# Link executable
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(RAYLIB_A) $(LIBS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(RAYLIB_INC) -c $< -o $@

# ============================================================================
# DIRECTORY CREATION
# ============================================================================
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ============================================================================
# UTILITY TARGETS
# ============================================================================
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXECUTABLE)

rebuild: clean all

help:
	@echo "Available targets:"
	@echo "  all      - Build the game (default)"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  RAYLIB_PREFIX - Path to raylib installation (default: /usr/local)"

# ============================================================================
# DEPENDENCIES
# ============================================================================
$(OBJ_DIR)/main2.o: main2.cpp ConsoleCapture.h ConfigParser.h TextureLoader.h UIRenderer.h Player.h GameConfig.h
$(OBJ_DIR)/ConsoleCapture.o: ConsoleCapture.cpp ConsoleCapture.h
$(OBJ_DIR)/ConfigParser.o: ConfigParser.cpp ConfigParser.h ConsoleCapture.h
$(OBJ_DIR)/TextureLoader.o: TextureLoader.cpp TextureLoader.h ConsoleCapture.h
$(OBJ_DIR)/UIRenderer.o: UIRenderer.cpp UIRenderer.h ConsoleCapture.h
$(OBJ_DIR)/Player.o: Player.cpp Player.h
$(OBJ_DIR)/GameConfig.o: GameConfig.cpp GameConfig.h