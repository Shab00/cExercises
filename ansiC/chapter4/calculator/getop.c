#include <ctype.h>
#include <stdio.h>
#include "calc.h"

int getop(char s[]) {
    int i, c;
    while (line[li] == '\0') {
        if (my_getline(line, MAXLINE) == 0)
            return EOF;
        li = 0;
    }

    while ((s[0] = c = line[li++]) == ' ' || c == '\t')
        ;
    s[1] = '\0';

    if (!isdigit(c) && c != '.') {
        return c; // not a number
    }

    i = 0;
    if (isdigit(c)) {
        while (isdigit(s[++i] = c = line[li++]))
            ;
    }
    if (c == '.') {
        while (isdigit(s[++i] = c = line[li++]))
            ;
    }
    s[i] = '\0';
    if (c != '\0')
        li--;

    return NUMBER;
}
