RAYLIB_PATH = /opt/homebrew/opt/raylib

CC = gcc

CFLAGS = -Wall -std=c99 -O2 -Iinclude -I$(RAYLIB_PATH)/include -MMD -MP
DEBUG_CFLAGS = -Wall -std=c99 -g -O0 -Iinclude -I$(RAYLIB_PATH)/include -MMD -MP

LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib \
	-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = game
BUILD_DIR = build

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:.o=.d)

all: $(BUILD_DIR)/$(TARGET)

# Link
$(BUILD_DIR)/$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

-include $(DEP)

run: all
	./$(BUILD_DIR)/$(TARGET)

debug: CFLAGS = $(DEBUG_CFLAGS)
debug: clean all

lldb: debug
	lldb ./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
