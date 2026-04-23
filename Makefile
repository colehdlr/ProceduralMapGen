RAYLIB_PATH = /opt/homebrew/opt/raylib

CC = gcc
CFLAGS = -Wall -std=c99 -O2 -I$(RAYLIB_PATH)/include -MMD -MP
LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib \
-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = game
SRC = main.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

-include $(DEP)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)
