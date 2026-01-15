#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int my_getline(char line[], int maxline);
int trim(char s[]);

int main()
{
    int max;
    char line[MAXLINE];
    char longest[MAXLINE];

    max = 0;
    while (my_getline(line, MAXLINE) > 0) {
        if (trim(line) > 0) {
            printf("%s", line);
        }
    }
    return 0;
}


int my_getline(char s[], int lim)
{
    int c;
    int total = 0;
    int stored = 0;

    while ((c = getchar()) != EOF) {
        ++total;
        if (stored < lim - 1) {
            s[stored++] = (char)c;
        }
        if (c == '\n')
            break;
    }

    if (total == 0 && c == EOF)
        return 0;

    s[stored] = '\0';
    return total;
}

int trim(char s[])
{
    int n = strlen(s);
    if (n == 0)
        return 0;

    int has_newline = 0;
    if (s[n - 1] == '\n') {
        has_newline = 1;
        --n;
    }

    while (n > 0 && (s[n - 1] == ' ' || s[n - 1] == '\t'))
        --n;

    if (n == 0) {
        s[0] = '\0';
        return 0;
    }

    if (has_newline) {
        s[n] = '\n';
        s[n + 1] = '\0';
        return n + 1;
    } else {
        s[n] = '\0';
        return n;
    }
}
