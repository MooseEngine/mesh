CC = gcc
CFLAGS = -Wall -Wextra -std=c99 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lm

TARGET = rasterizer
SRC = main.c lib/linalg.c lib/model.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean # tells make that all and clean are not actual files, just arbitrary targets

all: $(TARGET)

# Linking - separates the linking by building .o files
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# $@ refers to the value of $(TARGET)
# $^ refers to the value of all of the target's dependencies (in this case $(SRC)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
