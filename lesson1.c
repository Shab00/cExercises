#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

// Function to search for a pattern in a file
void search_file(const char *filename, const char *pattern) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            printf("%s: %s", filename, line);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <pattern> <filename>\n", argv[0]);
        return 1;
    }

    const char *pattern = argv[1];
    const char *filename = argv[2];

    search_file(filename, pattern);

    return 0;
}
