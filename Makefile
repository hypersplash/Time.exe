# ============================================================================
# PROJECT CONFIGURATION
# ============================================================================
# Force bash shell for cross-platform compatibility
SHELL := /bin/bash

TARGET = abandonedwhere
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -DNDEBUG -march=native -flto

# Debug build option: make DEBUG=1
ifdef DEBUG
    CXXFLAGS = -std=c++17 -Wall -Wextra -O0 -g -DDEBUG
    $(info Building in DEBUG mode)
else
    $(info Building in RELEASE mode)
endif

# ============================================================================
# PLATFORM DETECTION AND CONFIGURATION
# ============================================================================
# Detect platform automatically or allow override with PLATFORM=windows
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# Allow manual override: make PLATFORM=windows
PLATFORM ?= $(DETECTED_OS)

# Platform-specific settings
ifeq ($(PLATFORM),Windows)
    $(info Configuring for Windows (Msys2))
    TARGET := $(TARGET).exe
    
    # Detect Msys2 environment and set appropriate paths
    ifeq ($(MSYSTEM),CLANG64)
        $(info Using CLANG64 environment)
        RAYLIB_PREFIX ?= /clang64
    else ifeq ($(MSYSTEM),MINGW64)
        $(info Using MINGW64 environment)
        RAYLIB_PREFIX ?= /mingw64
    else ifeq ($(MSYSTEM),MINGW32)
        $(info Using MINGW32 environment)
        RAYLIB_PREFIX ?= /mingw32
    else
        $(info Using default MinGW64 paths)
        RAYLIB_PREFIX ?= /mingw64
    endif
    
    # Windows-specific libraries for raylib
    ifeq ($(MSYSTEM),CLANG64)
        # Clang64 doesn't always support static-libgcc/libstdc++
        LIBS = -lopengl32 -lgdi32 -lwinmm -lole32 -loleaut32 -limm32 -lversion
    else
        # MinGW supports static linking
        LIBS = -lopengl32 -lgdi32 -lwinmm -lole32 -loleaut32 -limm32 -lversion -static-libgcc -static-libstdc++
    endif
    # Optional: Add console window for debug builds on Windows
    ifdef DEBUG
        CXXFLAGS += -mconsole
    else
        CXXFLAGS += -mwindows
    endif
else
    $(info Configuring for Linux/Unix)
    # Linux/Unix libraries for raylib
    LIBS = -lGL -lm -lpthread -ldl -lrt -lX11
    RAYLIB_PREFIX ?= /usr/local
endif

# ============================================================================
# DIRECTORIES
# ============================================================================
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = .

# ============================================================================
# RAYLIB CONFIGURATION
# ============================================================================
# Set raylib paths based on detected environment
RAYLIB_INC = $(RAYLIB_PREFIX)/include
RAYLIB_LIBDIR = $(RAYLIB_PREFIX)/lib
RAYLIB_A = $(RAYLIB_LIBDIR)/libraylib.a

ifeq ($(PLATFORM),Windows)
    # Try pkg-config first, fallback to manual paths
    RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null || echo "-I$(RAYLIB_INC)")
    RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null || echo "$(RAYLIB_A)")
else
    # Linux/Unix configuration
    RAYLIB_CFLAGS = -I$(RAYLIB_INC)
    RAYLIB_LIBS = $(RAYLIB_A)
endif

# ============================================================================
# SOURCE FILES
# ============================================================================
SOURCES = main.cpp \
          ConsoleCapture.cpp \
          ConfigParser.cpp \
          TextureLoader.cpp \
          UIRenderer.cpp \
          Player.cpp \
          GameConfig.cpp \
          GameState.cpp

OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(BIN_DIR)/$(TARGET)

# ============================================================================
# PARALLEL BUILDS
# ============================================================================
# Enable parallel builds by default
MAKEFLAGS += -j$(shell nproc 2>/dev/null || echo 1)

# ============================================================================
# BUILD RULES
# ============================================================================
.PHONY: all clean rebuild help debug release install windows linux

# Default target
all: $(EXECUTABLE)

# Platform-specific shortcuts
windows:
	$(MAKE) PLATFORM=Windows

linux:
	$(MAKE) PLATFORM=Linux

# Debug build shortcut
debug:
	$(MAKE) DEBUG=1

# Release build shortcut  
release:
	$(MAKE) clean
	$(MAKE)

# Link executable with LTO support
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking $(TARGET) for $(PLATFORM)..."
ifeq ($(PLATFORM),Windows)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(RAYLIB_LIBS) $(LIBS)
else
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(RAYLIB_A) $(LIBS)
endif
	@echo "Build complete: $@"

# Compile source files with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
ifeq ($(PLATFORM),Windows)
	$(CXX) $(CXXFLAGS) $(RAYLIB_CFLAGS) -MMD -MP -c $< -o $@
else
	$(CXX) $(CXXFLAGS) -I$(RAYLIB_INC) -MMD -MP -c $< -o $@
endif

# ============================================================================
# DIRECTORY CREATION
# ============================================================================
$(OBJ_DIR):
	@test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@test -d $(BIN_DIR) || mkdir -p $(BIN_DIR)

# ============================================================================
# UTILITY TARGETS
# ============================================================================
clean:
	@echo "Cleaning build files..."
	@test -d $(OBJ_DIR) && rm -rf $(OBJ_DIR) || true
	@test -f $(EXECUTABLE) && rm -f $(EXECUTABLE) || true
	@echo "Clean complete."

rebuild: clean all

install: $(EXECUTABLE)
ifeq ($(PLATFORM),Windows)
	@echo "Installing to C:/msys64/usr/local/bin/..."
	cp $(EXECUTABLE) /usr/local/bin/
else
	@echo "Installing to /usr/local/bin..."
	sudo cp $(EXECUTABLE) /usr/local/bin/
endif
	@echo "Installation complete."

help:
	@echo "Available targets:"
	@echo "  all       - Build the game (default)"
	@echo "  windows   - Build for Windows (Msys2)"
	@echo "  linux     - Build for Linux"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Clean build optimized for release"
	@echo "  clean     - Remove build files"
	@echo "  rebuild   - Clean and build"
	@echo "  install   - Install to system"
	@echo "  help      - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  DEBUG=1         - Enable debug build"
	@echo "  PLATFORM=Windows/Linux - Force platform"
	@echo "  RAYLIB_PREFIX   - Path to raylib installation (default: /usr/local)"
	@echo ""
	@echo "Examples:"
	@echo "  make windows    - Build for Windows"
	@echo "  make DEBUG=1    - Debug build for current platform"
	@echo "  make windows DEBUG=1 - Debug build for Windows"

# ============================================================================
# DEPENDENCY TRACKING
# ============================================================================
# Include dependency files (auto-generated by -MMD -MP)
-include $(OBJECTS:.o=.d)

# ============================================================================
# EXPLICIT DEPENDENCIES (for reference)
# ============================================================================
$(OBJ_DIR)/main.o: main.cpp ConsoleCapture.h ConfigParser.h TextureLoader.h UIRenderer.h Player.h GameConfig.h GameState.h
$(OBJ_DIR)/ConsoleCapture.o: ConsoleCapture.cpp ConsoleCapture.h
$(OBJ_DIR)/ConfigParser.o: ConfigParser.cpp ConfigParser.h ConsoleCapture.h
$(OBJ_DIR)/TextureLoader.o: TextureLoader.cpp TextureLoader.h ConsoleCapture.h
$(OBJ_DIR)/UIRenderer.o: UIRenderer.cpp UIRenderer.h ConsoleCapture.h Version.h
$(OBJ_DIR)/Player.o: Player.cpp Player.h
$(OBJ_DIR)/GameConfig.o: GameConfig.cpp GameConfig.h
$(OBJ_DIR)/GameState.o: GameState.cpp GameState.h