#include <stdio.h>
#include "calc.h"

int my_getline(char s[], int lim) {
    int c, i, j;
    j = 0;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        if (i < lim - 2) {
            s[i] = c;
            ++j;
        }
    }

    if (c == '\n') {
        s[j] = c;
        ++j;
        ++i;
    }
    s[j] = '\0';
    return i;
}
