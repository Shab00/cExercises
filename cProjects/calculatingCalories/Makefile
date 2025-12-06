# Compiler and Flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BIN_DIR = bin

# Main program (mylib skeleton)
LIB_NAME = libmylib.a
EXEC_NAME = myprogram

# Calories program (elliptical/hfcal)
HFCAL_LIB_NAME = libhfcal.a
HFCAL_EXEC_NAME = elliptical

# Default target builds both executables
all: $(BIN_DIR)/$(EXEC_NAME) $(BIN_DIR)/$(HFCAL_EXEC_NAME)

$(LIB_DIR)/$(LIB_NAME): $(SRC_DIR)/mylib.o
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(BIN_DIR)/$(EXEC_NAME): $(LIB_DIR)/$(LIB_NAME) $(SRC_DIR)/main.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/main.o -L$(LIB_DIR) -lmylib


$(SRC_DIR)/hfcal.o: $(SRC_DIR)/hfcal.c $(INCLUDE_DIR)/hfcal.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_DIR)/$(HFCAL_LIB_NAME): $(SRC_DIR)/hfcal.o
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(SRC_DIR)/elliptical.o: $(SRC_DIR)/elliptical.c $(INCLUDE_DIR)/hfcal.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/$(HFCAL_EXEC_NAME): $(LIB_DIR)/$(HFCAL_LIB_NAME) $(SRC_DIR)/elliptical.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/elliptical.o -L$(LIB_DIR) -lhfcal


$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(SRC_DIR)/*.o $(LIB_DIR)/*.a $(BIN_DIR)/*

.PHONY: all clean
