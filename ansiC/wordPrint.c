#include <stdio.h>
#include <ctype.h>

int main(void)
{
    int ch;
    int in_word = 0;

    while ((ch = getchar()) != EOF) {
        if (!isspace((unsigned char)ch)) {
            putchar(ch);
            in_word = 1;
        } else if (in_word) {
            putchar('\n');
            in_word = 0;
        }
    }

    if (in_word)
        putchar('\n');

    return 0;
}
