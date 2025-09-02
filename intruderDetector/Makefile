CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11 $(shell pkg-config --cflags opencv4)
LDFLAGS = $(shell pkg-config --libs opencv4)
SRC_DIR = src
BIN_DIR = bin
EXEC = $(BIN_DIR)/intruderDetector
SRC = $(SRC_DIR)/main.c

all: $(EXEC)

$(EXEC): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all clean
