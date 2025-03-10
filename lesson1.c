#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glob.h>  // For glob patterns
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>  // For home directory expansion
#include <unistd.h>

#define MAX_PATTERNS 10  // Maximum number of patterns
#define MAX_FILES 100  // Maximum number of files/directories
#define MAX_LINE_LENGTH 256  // Maximum length of a line in a file

// Function to perform case-insensitive substring search
int strstr_case_insensitive(const char *haystack, const char *needle, int case_insensitive) {
    if (case_insensitive) {
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
    } else {
        return strstr(haystack, needle) != NULL;
    }
    return 0;  // No match
}

// Function to search for patterns in a file
int search_files(const char *filename, const char **patterns, int pattern_count, int case_insensitive, int show_line_numbers, int or_logic) {
    FILE *file = fopen(filename, "r");  // Open the file
    if (!file) {
        perror("Failed to open file");  // Print an error if the file can't be opened
        return 0;
    }

    char line[MAX_LINE_LENGTH];  // A buffer to store each line of the file
    int found = 0;  // Track if any match is found
    int line_number = 0;  // Track the current line number

    while (fgets(line, sizeof(line), file)) {  // Read the file line by line
        line_number++;
        int match = or_logic ? 0 : 1;  // Initialize match based on OR/AND logic

        for (int i = 0; i < pattern_count; i++) {  // Check each pattern
            if (strstr_case_insensitive(line, patterns[i], case_insensitive)) {
                if (or_logic) {
                    match = 1;  // For OR logic, one match is enough
                    break;
                }
            } else if (!or_logic) {
                match = 0;  // For AND logic, all patterns must match
                break;
            }
        }

        if (match) {  // If the line matches the patterns
            if (show_line_numbers) {
                printf("%s:%d: %s", filename, line_number, line);  // Print filename, line number, and line
            } else {
                printf("%s: %s", filename, line);  // Print filename and line
            }
            found = 1;  // Mark that a match was found
        }
    }

    fclose(file);  // Close the file when done
    return found;  // Return whether any match was found
}

// Function to recursively search directories
void search_directory(const char *dirpath, const char **patterns, int pattern_count, int case_insensitive, int show_line_numbers, int or_logic) {
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
            search_directory(path, patterns, pattern_count, case_insensitive, show_line_numbers, or_logic);  // Recursively search it
        } else if (S_ISREG(statbuf.st_mode)) {  // If the entry is a regular file
            search_files(path, patterns, pattern_count, case_insensitive, show_line_numbers, or_logic);  // Search the file
        }
    }

    closedir(dir);  // Close the directory when done
}

// Function to load allowed files from a configuration file
void load_allowed_files(const char *config_path, char **files, int *file_count) {
    FILE *config = fopen(config_path, "r");  // Open the configuration file
    if (!config) {
        perror("Failed to open config file");  // Print an error if the file can't be opened
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), config)) {  // Read the file line by line
        line[strcspn(line, "\n")] = 0;  // Remove the newline character
        if (*file_count >= MAX_FILES) {
            fprintf(stderr, "Error: Maximum number of files (%d) exceeded.\n", MAX_FILES);
            exit(1);
        }
        files[(*file_count)++] = strdup(line);  // Add the file/directory to the list
        if (!files[(*file_count) - 1]) {
            perror("Failed to allocate memory");
            exit(1);
        }
    }

    fclose(config);  // Close the configuration file
}

// Function to expand glob patterns (e.g., *.txt)
void expand_glob_patterns(const char *pattern, char **files, int *file_count) {
    glob_t glob_result;
    if (glob(pattern, GLOB_TILDE, NULL, &glob_result) == 0) {  // Expand the glob pattern
        for (size_t i = 0; i < glob_result.gl_pathc; i++) {  // Add each matching file/directory
            if (*file_count >= MAX_FILES) {
                fprintf(stderr, "Error: Maximum number of files (%d) exceeded.\n", MAX_FILES);
                exit(1);
            }
            files[(*file_count)++] = strdup(glob_result.gl_pathv[i]);  // Add the file/directory to the list
            if (!files[(*file_count) - 1]) {
                perror("Failed to allocate memory");
                exit(1);
            }
        }
    }
    globfree(&glob_result);  // Free the glob result
}

// Function to expand ~ to the home directory
const char *expand_home_directory(const char *path) {
    if (path[0] == '~') {  // Check if the path starts with ~
        const char *home = getenv("HOME");  // Get the home directory
        if (!home) {
            struct passwd *pw = getpwuid(getuid());  // Get the home directory from the password database
            if (pw) home = pw->pw_dir;
        }
        if (home) {
            static char expanded_path[MAX_LINE_LENGTH];
            snprintf(expanded_path, sizeof(expanded_path), "%s%s", home, path + 1);  // Expand ~ to the home directory
            return expanded_path;
        }
    }
    return path;  // Return the original path if ~ is not found
}

int main(int argc, char *argv[]) {
    if (argc < 2) {  // Check if the user provided enough arguments
        fprintf(stderr, "Usage: %s [-i] [-n] [-r] [--or] <pattern1> [pattern2 ...] <file1/dir1> [file2/dir2 ...]\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;  // Default: case-sensitive search
    int show_line_numbers = 0;  // Default: don't show line numbers
    int recursive = 0;  // Default: don't search directories recursively
    int or_logic = 0;  // Default: AND logic (all patterns must match)
    const char *patterns[MAX_PATTERNS];  // Array to store patterns
    int pattern_count = 0;  // Number of patterns
    char *files[MAX_FILES];  // Array to store files/directories
    int file_count = 0;  // Number of files/directories

    // Parse command-line options
    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;  // Enable case-insensitive search
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;  // Enable line numbers
        } else if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;  // Enable recursive directory search
        } else if (strcmp(argv[i], "--or") == 0) {
            or_logic = 1;  // Enable OR logic
        }
        i++;
    }

    // Parse patterns
    while (i < argc && pattern_count < MAX_PATTERNS) {
        patterns[pattern_count++] = argv[i++];
    }

    // Load allowed files from configuration
    const char *config_path = expand_home_directory("~/.tagfind");  // Expand ~ to the home directory
    load_allowed_files(config_path, files, &file_count);

    // Parse file arguments and expand glob patterns
    while (i < argc && file_count < MAX_FILES) {
        expand_glob_patterns(argv[i++], files, &file_count);
    }

    // Search files/directories
    int any_found = 0;
    for (int j = 0; j < file_count; j++) {
        struct stat statbuf;
        if (stat(files[j], &statbuf) == -1) {  // Get information about the file/directory
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode) && recursive) {  // If it's a directory and recursive search is enabled
            search_directory(files[j], patterns, pattern_count, case_insensitive, show_line_numbers, or_logic);  // Search the directory
        } else if (S_ISREG(statbuf.st_mode)) {  // If it's a regular file
            if (search_files(files[j], patterns, pattern_count, case_insensitive, show_line_numbers, or_logic)) {
                any_found = 1;  // Mark that a match was found
            }
        }
    }

    if (!any_found) {  // If no matches were found
        printf("No matches found for specified patterns.\n");
    }

    // Free allocated memory
    for (int j = 0; j < file_count; j++) {
        free(files[j]);
    }

    return 0;
}
