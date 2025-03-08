#include <stdio.h>
#include <string.h>
#include <ctype.h>  // For tolower()

#define MAX_LINE_LENGTH 256

// Function to perform case-insensitive substring search
int strstr_case_insensitive(const char *haystack, const char *needle) {
    while (*haystack) {  // Loop through the haystack (the line)
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower(*h) == tolower(*n)) {  // Compare characters case-insensitively
            h++;
            n++;
        }
        if (!*n) return 1;  // If we reached the end of the needle, it's a match
        haystack++;
    }
    return 0;  // No match found
}

// Function to search for a pattern in a file
void search_file(const char *filename, const char *pattern, int case_insensitive, int show_line_numbers) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (!file) {
        perror("Failed to open file");  // Print an error if the file can't be opened
        return;
    }

    char line[MAX_LINE_LENGTH];  // A buffer to store each line of the file
    int line_number = 0;  // Track the current line number

    while (fgets(line, sizeof(line), file)) {  // Read the file line by line
        line_number++;
        int match = 0;

        // Check for a match (case-sensitive or case-insensitive)
        if (case_insensitive) {
            match = strstr_case_insensitive(line, pattern);  // Use case-insensitive search
        } else {
            match = strstr(line, pattern) != NULL;  // Use case-sensitive search
        }

        if (match) {  // If the pattern is found in the line
            if (show_line_numbers) {
                printf("%s:%d: %s", filename, line_number, line);  // Print filename, line number, and line
            } else {
                printf("%s: %s", filename, line);  // Print filename and line
            }
        }
    }

    fclose(file);  // Close the file when done
}

int main(int argc, char *argv[]) {
    if (argc < 3) {  // Check if the user provided enough arguments
        fprintf(stderr, "Usage: %s [-i] [-n] <pattern> <filename>\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;  // Default: case-sensitive search
    int show_line_numbers = 0;  // Default: don't show line numbers
    int i = 1;

    // Parse command-line options
    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;  // Enable case-insensitive search
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;  // Enable line numbers
        }
        i++;
    }

    const char *pattern = argv[i++];  // The pattern to search for
    const char *filename = argv[i];  // The file to search in

    search_file(filename, pattern, case_insensitive, show_line_numbers);  // Call the search function

    return 0;
}
