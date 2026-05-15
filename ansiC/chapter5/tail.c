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

    if (argc == 1) {
        n = DEFAULT_N; 
    } else if (argc == 2 && argv[1][0] == '-') {
        n = atoi(argv[1] + 1);
        if (n < 1) {
            error("invalid value n");
        }
    } else {
        error("usage: wrong");
    }

    for (i = 0; i < MAXLINES; i++) {
        lines[i] = NULL;
    }
    while ((len = getline_custom(line, MAXLEN)) > 0) {
        if (p + len + 1 >= bufend)
            p = buffer;
        lines[last] = p;
        strcpy(p, line);
        p += len + 1;
        last = (last + 1) % MAXLINES;
        nlines++;
    }

    if (n > nlines)
        n = nlines;
    first = last - n;
    if (first < 0)
        first += MAXLINES;

    for (i = first; n-- > 0; i = (i + 1) % MAXLINES)
        printf("%s", lines[i]);

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
