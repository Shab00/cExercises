#include <stdio.h>
#define MAXLINE 1000

int my_getline(char line[], int maxline);
void copy(char to[], char from[]);

int main()
{
    int len;
    int max;
    char line[MAXLINE];
    char longest[MAXLINE];

    max = 0;
    while ((len = my_getline(line, MAXLINE)) > 0) {
        if (len > 80) {
            printf("%d, %s", len, line);
        }
        if (len > max) {
            max = len;
            copy(longest, line);
        }
    }

    if (max > 0) {
        printf("%s", longest);
        }
    return 0;
}

int my_getline(char s[], int lim)
{
    int c, i;
    i = 0;
    while (i < lim - 1) {
        if ((c = getchar()) == EOF) {
            break;
        }
        if (c == '\n') {
            s[i++] = c;
            break;
        }
        s[i++] = c;
    }
    s[i] = '\0';
    return i;
}

void copy(char to[], char from[])
{
    int i;

    i = 0;
    while ((to[i] = from[i]) != '\0') {
        ++i;
    }
}
