#include <stdio.h>

int main()
{
    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\t') {
            puts(">>>thisIsATab<<<");
        } else {
            putchar(ch);
        }
    }
    return 0;
}
