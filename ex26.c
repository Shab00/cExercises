#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glob.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATTERNS 10
#define MAX_FILES 100
#define MAX_LINE_LENGTH 256

int strstr_case_insensitive(const char *haystack, const char *needle, int case_insensitive) {
    if (case_insensitive) {
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
    } else {
        return strstr(haystack, needle) != NULL;
    }
    return 0;
}

int search_files(const char *filename, const char **patterns, int pattern_count,  int case_insensitive, int show_line_numbers, int or_logic) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;
    int line_number = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        int match = or_logic ? 0 : 1;

        for (int i = 0; i < pattern_count; i++) {
            if (strstr_case_insensitive(line, patterns[i], case_insensitive)) {
                if (or_logic) {
                    match = 1;
                    break;
                }
            } else if (!or_logic) {
                match =0;
                break;
            }
        }
    
        if (match) {
            if (show_line_numbers) {
                printf("%s:%d: %s", filename, line_number, line);
            } else {
                printf("%s: %s", filename, line); 
            }
            found = 1;
        }
    }

    fclose(file);
    return found;
}

void search_directory(const char *dirpath, const char **patterns, int pattern_count, int case_insensitive, int show_line_numbers, int or_logic) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip current and parent directory
        }

        char path[MAX_LINE_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            search_directory(path, patterns, pattern_count, case_insensitive, show_line_numbers, or_logic);  // Recursively search subdirectories
        } else if (S_ISREG(statbuf.st_mode)) {
            search_files(path, patterns, pattern_count, case_insensitive, show_line_numbers, or_logic);  // Search regular files
        }
    }

    closedir(dir);
}

void load_allowed_files(const char *config_path, char **files, int *file_count) {
    FILE *config = fopen(config_path, "r");
    if (!config) {
        perror("Failed to ope config file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), config)) {
        line[strcspn(line, "\n")] = 0;
        files[(*file_count)++] = strdup(line);
    }

    fclose(config);
}

void expand_glob_patterns(const char *pattern, char **files, int *file_count) {
    glob_t glob_result;
    glob(pattern, GLOB_TILDE, NULL, &glob_result);

    for (size_t i = 0; i < glob_result.gl_pathc; i++) {
        files[(*file_count)++] = strdup(glob_result.gl_pathv[i]);
    }

    globfree(&glob_result);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-i] [-n] [-r] [--or]  <pattern> [pattern2 ...] <file1/dir1> [file2/dir2 ...]\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;
    int show_line_numbers = 0;
    int recursive = 0;
    int or_logic = 0;
    const char *patterns[MAX_PATTERNS];
    int pattern_count = 0;
    char *files[MAX_FILES];
    int file_count = 0;

    // Parse command-line options
    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
        } else if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;
        } else if (strcmp(argv[i], "--or") == 0 ){
            or_logic = 1;
        }
        i++;
    }

    while (i < argc && pattern_count < MAX_PATTERNS) {
        patterns[pattern_count++] = argv[i++];
    }

    load_allowed_files("~/.tagfind", files, &file_count);

    while (i < argc && file_count < MAX_FILES) {
        expand_glob_patterns(argv[i++], files, &file_count);
    }

    int any_found = 0;
    for (int j = 0; j < file_count; j++) {
        struct stat statbuf;
        if (stat(files[j], &statbuf)) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode) && recursive) {
            search_directory(files[j], patterns, pattern_count, case_insensitive, show_line_numbers, or_logic); 
        } else if (S_ISREG(statbuf.st_mode)) {
            if (search_files(files[j], patterns, pattern_count, case_insensitive, show_line_numbers, or_logic)) {
                any_found = 1;
            }
        }       
    }

    if (!any_found) {
        printf("No matches found for specified patterns.\n");
    }
    
    for (int j = 0; j < file_count; j++) {
        free(files[j]);
    }

    return 0;
}
