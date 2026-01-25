#include <stdio.h>
#define TABSTOP 8

int main()
{
    int col, spc, c, to_tab;
    col = spc = c = to_tab = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ') {
            spc++;
        } else {
            while (spc > 0) {
                to_tab = TABSTOP - (col % TABSTOP);
                if (spc >= to_tab) {
                    putchar('\t');
                    col += to_tab;
                    spc -= to_tab;
                } else {
                    putchar(' ');
                    col++;
                    spc--;
                }
            }
            if (c == '\n') {
                putchar('\n');
                col = 0;
            } else {
                putchar(c);
                col++;
            }
        }
    }
    return 0;
}
