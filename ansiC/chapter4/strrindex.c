#include <stdio.h>
#define MAXLINE 1000

int my_getline(char line[], int max);
int strrindex(char s[], char t[]);

char pattern[] = "ould";

int main()
{
    char line[MAXLINE];
    int found = 0;

    while (my_getline(line, MAXLINE) > 0){
        if (strrindex(line, pattern) >= 0) {
            printf("%s", line);
            found++;
        }
    }
    return found;
}

int my_getline(char s[], int lim)
{
    int c, i;
    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n') {
        s[i++] = c;
    }
    if (c == '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
    return i;
}

int strrindex(char s[], t[])
{
    return 1;
}
