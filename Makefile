# =========================================================
# Config
# =========================================================
TARGET_OS ?= linux

# Project folders
SRC_DIR := src
OBJ_DIR := obj/$(TARGET_OS)
BIN_DIR := bin
INCLUDE_DIR := include

# Sources/objects
SRC  := $(wildcard $(SRC_DIR)/*.cpp)
OBJ  := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEPS := $(OBJ:.o=.d)

# =========================================================
# Compiler & flags per target
# =========================================================
ifeq ($(TARGET_OS),linux)
  CXX      := g++
  TARGET   := $(BIN_DIR)/game
  CXXFLAGS := -Wall -std=c++17 -I$(INCLUDE_DIR) -MMD -MP
  LDFLAGS  := -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
else ifeq ($(TARGET_OS),windows)
  CXX      := x86_64-w64-mingw32-g++
  TARGET   := $(BIN_DIR)/game.exe
  CXXFLAGS := -Wall -std=c++17 -I$(INCLUDE_DIR) -Iexternal/windows/include -MMD -MP
  LDFLAGS  := -Lexternal/windows/lib -l:libraylib.a -lopengl32 -lgdi32 -lwinmm -static
else
  $(error Unknown TARGET_OS='$(TARGET_OS)'. Use TARGET_OS=linux or TARGET_OS=windows)
endif

# =========================================================
# Rules
# =========================================================
.PHONY: all linux windows clean

all: $(TARGET)
	cp -r res $(BIN_DIR)

linux:
	@$(MAKE) TARGET_OS=linux

windows:
	@$(MAKE) TARGET_OS=windows

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf obj $(BIN_DIR)

# Include dependency files
-include $(DEPS)
