# ======================
# Project configuration
# ======================

CC = gcc
TARGET = game
BUILD_DIR = build

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:.o=.d)

UNAME_S := $(shell uname -s)

# ======================
# Platform-specific config
# ======================

# ---------- macOS ----------
ifeq ($(UNAME_S),Darwin)

RAYLIB_PATH = /opt/homebrew/opt/raylib

CFLAGS = -Wall -std=c99 -O2 -Iinclude -I$(RAYLIB_PATH)/include -MMD -MP
DEBUG_CFLAGS = -Wall -std=c99 -g -O0 -Iinclude -I$(RAYLIB_PATH)/include -MMD -MP

LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib \
	-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

endif

# ---------- Linux (CI / Ubuntu) ----------
ifeq ($(UNAME_S),Linux)

CFLAGS = -Wall -std=c99 -O2 -Iinclude $(shell pkg-config --cflags raylib) -MMD -MP
DEBUG_CFLAGS = -Wall -std=c99 -g -O0 -Iinclude $(shell pkg-config --cflags raylib) -MMD -MP

LDFLAGS = $(shell pkg-config --libs raylib)

endif

# ======================
# Build rules
# ======================

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(DEP)

# ======================
# Utilities
# ======================

run: all
	./$(BUILD_DIR)/$(TARGET)

debug: CFLAGS = $(DEBUG_CFLAGS)
debug: clean all

lldb: debug
	lldb ./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
