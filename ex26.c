#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int search_files(const char *filename, const char *pattern, int case_insensitive) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }

    char line[256];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        char *line_ptr = line;

        while (*line_ptr) {
            char *line_sub_ptr = line_ptr;
            const char *pattern_ptr = pattern;
            int match = 1;

            while (*line_sub_ptr && *pattern_ptr) {
                if (case_insensitive ? (tolower(*line_sub_ptr) != tolower(*pattern_ptr)) : (*line_sub_ptr != *pattern_ptr)) {
                    match = 0;
                    break;
                }
                line_sub_ptr++;
                pattern_ptr++;
            }

            if (match && !*pattern_ptr) {
                printf("%s: %s", filename, line);
                found = 1;
                break;  
            }
            line_ptr++;
        }
    }

    fclose(file);
    return found;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [-i] <pattern> <file1> [<file2> ...]\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;
    const char *pattern;
    int file_start_index = 2;

    if (strcmp(argv[1], "-i") == 0) {
        case_insensitive = 1;
        pattern = argv[2];
        file_start_index = 3;
    } else {
        pattern = argv[1];
    }

    int any_found = 0;
    for (int i = file_start_index; i < argc; i++) {
        if (search_files(argv[i], pattern, case_insensitive)) {
            any_found = 1;
        }
    }

    if (!any_found) {
        printf("No matches found for '%s' in the specified files.\n", pattern);
    }

    return 0;
}
