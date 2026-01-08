#include <stdio.h>
#include <ctype.h>

int main(void) 
{
    int c, i;
    int letterCount[26] = {0};

    while ((c = getchar()) != EOF) {
        if (isalpha((unsigned char)c)) {
            c = tolower((unsigned char)c);
            ++letterCount[c - 'a'];
        }
    }

    for (i = 0; i < 26; ++i) {
        printf("%c: %d\n", 'a' + i, letterCount[i]);
    }
    return 0;
}
