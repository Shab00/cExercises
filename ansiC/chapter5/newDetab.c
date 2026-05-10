#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1000
#define TABINC 8
#define YES 1
#define NO 0

void settab(int argc, char *argv[], char *tab);
int tabpos(int pos, char *tab);

int main(int argc, char *argv[]) {
    char tab[MAXLINE+1];
    int c, pos = 1;

    settab(argc, argv, tab);

    while ((c = getchar()) != EOF) {
        if (c == '\t') {
            do {
                putchar(' ');
                ++pos;
            } while (tabpos(pos, tab) == NO);
        } else if (c == '\n') {
            putchar(c);
            pos = 1;
        } else {
            putchar(c);
            ++pos;
        }
    }
    return 0;
}

void settab(int argc, char *argv[], char *tab) {
    int i, pos;

    for (i = 1; i <= MAXLINE; i++)
        tab[i] = NO;

    if (argc <= 1) {
        for (i = TABINC; i <= MAXLINE; i += TABINC)
            tab[i] = YES;
    } else {
        while (--argc > 0) {
            pos = atoi(*++argv);
            if (pos > 0 && pos <= MAXLINE)
                tab[pos] = YES;
        }
    }
}

int tabpos(int pos, char *tab) {
    if (pos > MAXLINE)
        return YES;
    else
        return tab[pos];
}
