#include <stdio.h>
#include <ctype.h>
#define MAXSTARS 50

static void print_stars(int n)
{
    for (int j = 0; j < n; ++j)
        putchar('*');
}

int main(void) 
{
    int c, i;
    int letterCount[26] = {0};
    int maxvalue = 0;

    while ((c = getchar()) != EOF) {
        if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c);
            ++letterCount[c - 'a'];
        }
    }

    for (i = 0; i < 26; ++i) {
        if (letterCount[i] > maxvalue)
            maxvalue = letterCount[i];
    }

    for (i = 0; i < 26; ++i) {
        int len = 0;
        if (maxvalue > 0) {
            len = letterCount[i] * MAXSTARS / maxvalue;
        }

        if (letterCount[i] > 0 && len == 0) {
            len = 1;
        }

        printf("%c: %5d ", 'a' + i, letterCount[i]);
        print_stars(len);
        putchar('\n');
    }
    return 0;
}
