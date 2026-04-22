RAYLIB_PATH = /opt/homebrew/opt/raylib

CC = gcc
CFLAGS = -Wall -std=c99 -O2 -I$(RAYLIB_PATH)/include
LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib \
-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = game
SRC = main.c

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
