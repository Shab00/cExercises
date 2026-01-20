#include <stdio.h>
#define TABSTOP 8
#define MAXLINE 1000

int my_getline(char line[], int maxline);

int main()
{
    int column, ch, len;
    char line[MAXLINE];

    column = 0;

    while ((len = my_getline(line, MAXLINE)) > 0) {
        printf("%d, %s", len, line);
    }
    return 0;
}

int my_getline(char s[], int lim)
{
    int c, i, j, column;
    column = 0;
    j = 0;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        if (c == '\t') {
            int spaces = TABSTOP - (column % TABSTOP);
            for (int k = 0; k < spaces; ++k) {
                s[j++] = ' ';
                column++;
            }
        } else {
            s[j++] = c;
            column++;
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
