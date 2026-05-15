#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_N 10
#define MAXLINES 100
#define MAXLEN 100

void error(char *msg);
int getline_custom(char *s, int lim);

int main(int argc, char *argv[]) {
    int n = DEFAULT_N;
    char *lines[MAXLINES];
    char buffer[MAXLINES * MAXLEN];
    char *bufend = buffer + sizeof(buffer);
    char *p = buffer;
    char line[MAXLEN];
    int last = 0;
    int nlines = 0;
    int first, i, len;

    // --- Parse arguments for -n ---
    /*
        if argc == 1: 
            use default n
        else if argc == 2 and argv[1] starts with '-': 
            parse n from argv[1]
        else: 
            error and usage
    */
    if (argc == 1) {
        n = DEFAULT_N 
    } else if (argc == 2 && argv[1][0] == '-') {
        n = argv[1];
    } else {
        error(argv[1]);
    }
    // --- Initialize lines[] to NULL ---
    /*
        For robustness. (not strictly required)
    */

    // --- Allocate buffer and initialize pointers ---
    /*
        (buffer is already allocated statically above)
    */

    // --- Input Loop: Read each line until EOF ---
    /*
        while (next line read into line[]):
            - If not enough buffer left, wrap p to buffer start
            - Save pointer in lines[last]
            - Copy line[] into buffer at p
            - Increment (and wrap) last in circular fashion (last = (last+1) % MAXLINES)
            - Advance pointer p by line length
            - Increment nlines
    */

    // --- Calculate number of lines to print & first line index ---
    /*
        n = min(n, nlines)
        first = last - n
        if (first < 0) first += MAXLINES
    */

    // --- Print last n lines in correct order ---
    /*
        for i = first, count = 0; count < n; i = (i + 1) % MAXLINES:
            print lines[i]
    */

    return 0;
}

void error(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int getline_custom(char *s, int lim) {
    int c;
    char *start = s;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
        *s++ = c;
    }
    if (c == '\n') {
        *s++ = c;
    }
    *s = '\0';
    return s - start;
}
