CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11 $(shell pkg-config --cflags gstreamer-1.0)
LDFLAGS = $(shell pkg-config --libs gstreamer-1.0)
SRC_DIR = src
BIN_DIR = bin
EXEC = $(BIN_DIR)/intruderDetector
SRC = $(SRC_DIR)/main.c

all: $(EXEC)

$(EXEC): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@rm -rf $(BIN_DIR)/*

.PHONY: all clean
