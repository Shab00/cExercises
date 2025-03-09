#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>  // For directory handling
#include <sys/stat.h>  // For file status (e.g., checking if it's a directory)

#define MAX_LINE_LENGTH 256

// Function to perform case-insensitive substring search
int strstr_case_insensitive(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower(*h) == tolower(*n)) {
            h++;
            n++;
        }
        if (!*n) return 1;  // Match found
        haystack++;
    }
    return 0;  // No match
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

// Function to recursively search directories
void search_directory(const char *dirpath, const char *pattern, int case_insensitive, int show_line_numbers) {
    DIR *dir = opendir(dirpath);  // Open the directory
    if (!dir) {
        perror("Failed to open directory");  // Print an error if the directory can't be opened
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {  // Read each entry in the directory
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip the current directory (.) and parent directory (..)
        }

        char path[MAX_LINE_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);  // Create the full path to the entry

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {  // Get information about the entry
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {  // If the entry is a directory
            search_directory(path, pattern, case_insensitive, show_line_numbers);  // Recursively search it
        } else if (S_ISREG(statbuf.st_mode)) {  // If the entry is a regular file
            search_file(path, pattern, case_insensitive, show_line_numbers);  // Search the file
        }
    }

    closedir(dir);  // Close the directory when done
}

int main(int argc, char *argv[]) {
    if (argc < 3) {  // Check if the user provided enough arguments
        fprintf(stderr, "Usage: %s [-i] [-n] [-r] <pattern> <file1/dir1> [file2/dir2 ...]\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;  // Default: case-sensitive search
    int show_line_numbers = 0;  // Default: don't show line numbers
    int recursive = 0;  // Default: don't search directories recursively
    int i = 1;

    // Parse command-line options
    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;  // Enable case-insensitive search
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;  // Enable line numbers
        } else if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;  // Enable recursive directory search
        }
        i++;
    }

    const char *pattern = argv[i++];  // The pattern to search for

    // Search files/directories
    for (; i < argc; i++) {
        struct stat statbuf;
        if (stat(argv[i], &statbuf) == -1) {  // Get information about the file/directory
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode) && recursive) {  // If it's a directory and recursive search is enabled
            search_directory(argv[i], pattern, case_insensitive, show_line_numbers);  // Search the directory
        } else if (S_ISREG(statbuf.st_mode)) {  // If it's a regular file
            search_file(argv[i], pattern, case_insensitive, show_line_numbers);  // Search the file
        }
    }

    return 0;
}
