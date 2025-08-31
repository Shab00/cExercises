#!/bin/bash

# Usage: ./init_c_project.sh [project_name]
PROJECT_NAME=${1:-my_c_project}

# Create directory structure
mkdir -p "$PROJECT_NAME"/{src,include,lib,bin}

# Create example header file
cat > "$PROJECT_NAME/include/mylib.h" <<'EOF'
#ifndef MYLIB_H
#define MYLIB_H

void hello();

#endif // MYLIB_H
EOF

# Create example library source file
cat > "$PROJECT_NAME/src/mylib.c" <<'EOF'
#include <stdio.h>
#include "mylib.h"

void hello() {
    printf("Hello from the library!\\n");
}
EOF

# Create example main file
cat > "$PROJECT_NAME/src/main.c" <<'EOF'
#include "mylib.h"

int main() {
    hello();
    return 0;
}
EOF

# Create Makefile
cat > "$PROJECT_NAME/Makefile" <<'EOF'
# Compiler and Flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c11

# Directories
SRC_DIR = src
INCLUDE_DIR = include
LIB_DIR = lib
BIN_DIR = bin

# Targets
LIB_NAME = libmylib.a
EXEC_NAME = myprogram

SRC_FILES = \$(wildcard \$(SRC_DIR)/*.c)
OBJ_FILES = \$(SRC_FILES:.c=.o)
LIB_FILE = \$(LIB_DIR)/\$(LIB_NAME)

all: \$(BIN_DIR)/\$(EXEC_NAME)

\$(LIB_FILE): \$(SRC_DIR)/mylib.o
	mkdir -p \$(LIB_DIR)
	ar rcs \$@ \$^

\$(BIN_DIR)/\$(EXEC_NAME): \$(LIB_FILE) \$(SRC_DIR)/main.o
	mkdir -p \$(BIN_DIR)
	\$(CC) \$(CFLAGS) -o \$@ \$(SRC_DIR)/main.o -L\$(LIB_DIR) -lmylib

\$(SRC_DIR)/%.o: \$(SRC_DIR)/%.c
	\$(CC) \$(CFLAGS) -c \$< -o \$@

clean:
	rm -f \$(SRC_DIR)/*.o \$(LIB_DIR)/*.a \$(BIN_DIR)/*

.PHONY: all clean
EOF

# Create README
cat > "$PROJECT_NAME/README.md" <<'EOF'
# My C Project Skeleton

## Directory Layout

- `include/` — Header files
- `src/` — Source files
- `lib/` — Static libraries (.a)
- `bin/` — Executables

## Build and Run

```sh
make
./bin/myprogram
