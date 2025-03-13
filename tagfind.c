#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

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

int search_file(const char *filename, const char *pattern, int case_insensitive, int show_line_numbers) {
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
        if (strstr_case_insensitive(line, pattern, case_insensitive)) {
            if (show_line_numbers) {
                printf("%s:%d %s", filename, line_number, line);
            } else {
                printf("%s: %s", filename, line);
            }
            found = 1;
        }
    }

    fclose(file);
    return found;
}

void search_directory(const char *dirpath, const char *pattern, int case_insensitive, int show_line_numbers) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror("Failed to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; 
        }

        char path[MAX_LINE_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            search_directory(path, pattern, case_insensitive, show_line_numbers); 
        } else if (S_ISREG(statbuf.st_mode)) {
            search_file(path, pattern, case_insensitive, show_line_numbers);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-i] [-n] [-r] <pattern> <file1/dir1> [file2/dir2 ...]\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;
    int show_line_numbers = 0;
    int recursive = 0;
    int i = 1;

    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-i") == 0) {
            case_insensitive = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            show_line_numbers = 1;
        } else if (strcmp(argv[i], "-r") == 0) {
            recursive = 1;
        }
        i++;
    }

    const char *pattern = argv[i++]; 

    for (; i < argc; i++) {
        struct stat statbuf;
        if (stat(argv[i], &statbuf) == -1) {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode) && recursive) {
            search_directory(argv[i], pattern, case_insensitive, show_line_numbers);
        } else if (S_ISREG(statbuf.st_mode)) {
            search_file(argv[i], pattern, case_insensitive, show_line_numbers);
        }
    }

    return 0;
}
