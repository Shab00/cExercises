#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct island {
    char *name;
    char *opens;
    char *closes;
    struct island *next;
} island;

// Create a new island node
island* create(char *name) {
    // Implement this
}

// Free an island node
void free_island(island *i) {
    // Implement this
}

// Print all islands in the list
void print_islands(island *start) {
    // Implement this
}

int main() {
    // Implement main logic here
    return 0;
}
