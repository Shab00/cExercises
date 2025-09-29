CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11 $(shell pkg-config --cflags gstreamer-1.0 gstreamer-app-1.0)
LDFLAGS = $(shell pkg-config --libs gstreamer-1.0 gstreamer-app-1.0)
SRC_DIR = src
BIN_DIR = bin
EXEC1 = $(BIN_DIR)/intruderDetector
EXEC2 = $(BIN_DIR)/display
SRC1 = $(SRC_DIR)/main.c
SRC2 = $(SRC_DIR)/display.c

all: $(EXEC1) $(EXEC2)

$(EXEC1): $(SRC1)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(EXEC2): $(SRC2)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	@rm -rf $(BIN_DIR)/*

.PHONY: all clean
