CC=g++
CFLAGS=-Wall -Wextra -std=c++17

LIBS=-lSDL2 -lSDL2_image

SRC=src/main.cpp
BIN=bin/main

all: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LIBS)

clean:
	rm -f $(BIN)
