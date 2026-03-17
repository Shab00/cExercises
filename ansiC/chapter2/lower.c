#include <stdio.h>

int lower(int c);

int main()
{
    char line[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i = 0;
    for (; i < 26; i++) {
        printf("%c", lower(line[i]));
    }
    putchar('\n');
    return 0;
}

int lower(int c)
{
    return c = (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}
