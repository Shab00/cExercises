#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int my_getline(char line[], int maxline);
void reverse(char s[]);

int main(void)
{
    char line[MAXLINE];

    while (my_getline(line, MAXLINE) > 0) {
        reverse(line);
        fputs(line, stdout);
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

void reverse(char s[])
{
    int n = strlen(s);
    if (n == 0)
        return;

    int has_newline = 0;
    if (s[n - 1] == '\n') {
        has_newline = 1;
        --n;
    }

    int i = 0, j = n - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        ++i;
        --j;
    }

    if (has_newline) {
        s[n] = '\n';
        s[n + 1] = '\0';
    } else {
        s[n] = '\0';
    }
}
