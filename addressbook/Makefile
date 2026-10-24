# Compiler and Flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Werror -g -std=c11

# Directories
SRC_DIR = src
BIN_DIR = bin

# Targets
EXEC = $(BIN_DIR)/addressbook
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)

.PHONY: all clean
