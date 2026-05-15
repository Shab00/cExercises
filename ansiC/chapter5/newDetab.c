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
    int m = 1;
    int n = TABINC;
    int explicit = 0;

    for (i = 1; i <= MAXLINE; i++)
        tab[i] = NO;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            m = atoi(argv[i] + 1);
            if (m <= 0) m = 1;
        } else if (argv[i][0] == '+') {
            n = atoi(argv[i] + 1);
            if (n <= 0) n = TABINC;
        } else {
            pos = atoi(argv[i]);
            if (pos > 0 && pos <= MAXLINE) {
                tab[pos] = YES;
                explicit = 1;
            }
        }
    }

    if (explicit) {
        return;
    } else {
        for (i = m; i <= MAXLINE; i += n) {
            tab[i] = YES;
            
        }
    }
}

int tabpos(int pos, char *tab) {
    if (pos > MAXLINE) {
        return YES;
    } else {
        return tab[pos];
    }
}
