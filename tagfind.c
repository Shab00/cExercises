#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

int strstr_case_insensitive(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower(*h) == tolower(*n)) {
            h++;
            n++;
        }
        if (!*n) return 1;
        haystack++;
    }
    return 0;
}

void search_file(const char *filename, const char *pattern, int case_insensitive, int show_line_numbers) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        int match = 0;
        
        if (case_insensitive) {
            match = strstr_case_insensitive(line, pattern);
        } else {
            match = strstr(line, pattern) != NULL;
        }

        if (match) {
            if (show_line_numbers) {
                printf("%s: %d: %s", filename, line_number, line);
            } else {
                printf("%s: %s", filename, line);
            }
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <pattern> <filename>\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;
    int show_line_numbers = 0;
    int i = 1;

    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
        }
        i++;
    }

    const char *pattern = argv[i++];
    const char *filename = argv[i];

    search_file(filename, pattern, case_insensitive, show_line_numbers);

    return 0;
}
