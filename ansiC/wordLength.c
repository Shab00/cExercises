#include <stdio.h>
#include <ctype.h>

#define MAXHIST 15
#define MAXWORD 11

#define IN  1
#define OUT 0

int main(void)
{
    int c;
    int state = OUT;
    int nc = 0;
    int wl[MAXWORD];
    int i, overflow = 0, maxvalue = 0;

    for (i = 0; i < MAXWORD; ++i)
        wl[i] = 0;

    while ((c = getchar()) != EOF) {
        if (isspace((unsigned char)c)) {
            if (state == IN) {
                if (nc < MAXWORD)
                    ++wl[nc];
                else
                    ++overflow;
                nc = 0;
                state = OUT;
            }
        } else {
            state = IN;
            ++nc;
        }
    }

    if (state == IN) {
        if (nc < MAXWORD)
            ++wl[nc];
        else
            ++overflow;
    }

    for (i = 1; i < MAXWORD; ++i)
        if (wl[i] > maxvalue)
            maxvalue = wl[i];

    for (i = 1; i < MAXWORD; ++i) {
        int len = 0;
        if (maxvalue > 0)
            len = wl[i] * MAXHIST / maxvalue;
        if (wl[i] > 0 && len == 0)
            len = 1;

        printf("%2d (%4d): ", i, wl[i]);
        for (int j = 0; j < len; ++j)
            putchar('*');
        putchar('\n');
    }

    if (overflow > 0)
        printf("Overflow (length >= %d): %d\n", MAXWORD, overflow);

    return 0;
}
