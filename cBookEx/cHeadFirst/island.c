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
    island *i = malloc(sizeof(island));
    if (i == NULL) return NULL;
    i->name = strdup(name);
    i->opens = strdup("09:00");
    i->closes = strdup("17:00");
    i->next = NULL;
    return i;
}

// Free a whole list of islands
void free_islands(island *start) {
    island *i = start;
    while (i != NULL) {
        island *next = i->next;
        free(i->name);
        free(i->opens);
        free(i->closes);
        free(i);
        i = next;
    }
}

// Print all islands in the list
void print_islands(island *start) {
    for (island *i = start; i != NULL; i = i->next) {
        printf("Name: %s open: %s-%s\n", i->name, i->opens, i->closes);
    }
}

int main() {
    // Build the list using create()
    island *amity = create("Amity");
    island *craggy = create("Craggy");
    island *isla_nublar = create("Isla Nublar");
    island *skull = create("Skull");
    island *shutter = create("Shutter");

    // Link the islands
    amity->next = craggy;
    craggy->next = isla_nublar;
    isla_nublar->next = skull;
    skull->next = shutter;

    print_islands(amity);

    // Free memory
    free_islands(amity);

    return 0;
}
