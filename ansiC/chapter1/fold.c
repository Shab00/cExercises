#include <stdio.h>
#define TABSTOP 8
#define foldLim 20
#define MAXLINE 1000

int my_getline(char line[], int maxline);
void fold_line(char line[], int len, int lim);

int main()
{
    int len;
    char line[MAXLINE];
    while ((len = my_getline(line, MAXLINE)) > 0) {
        printf("len%d\noriginal:%s", len, line);
        fold_line(line, len, foldLim);
    }
}

int my_getline(char s[], int lim) 
{
    int c, i, j;
    j = 0;
        for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
            s[i] = c;
            ++j;
        }

    if (c == '\n') {
        s[j] = c;
        ++j;
        ++i;
    }
    s[j] = '\0';
    return i;
}

void fold_line(char s[], int len, int lim) 
{
    int i, j, lb, start;
    lb = -1;
    j = 0;
    start = 0;

    for (i = 0; i < len; i++) {
        j++;


        if (s[i] == ' ' || s[i] == '\t') {
            lb = i;
        }

        if (j >= lim) {
            if (lb != -1) {
                for (int k = start; k < lb; k++) {
                    putchar(s[k]);
                }
                putchar('\n');

                i = lb;
                while (s[i] == ' ' || s[i] == '\t') {
                    i++;
                }

                start = i;
                lb = -1;
                j = 0;
            } else {
                for (int k = start; k < i; k++) {
                    putchar(s[k]);
                }
                putchar('\n');

                start = i;
                j = 0;
            }
        }
    }

    for (int k = start; k < len; k++) {
        putchar(s[k]);
    }
}
