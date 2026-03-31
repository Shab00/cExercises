#include <stdio.h>
#include <string.h>

void expand(char s1[], char s2[]);

int main()
{
    char result[1000];
    char out[1000];
    char in[1000] = "a-z";
    expand(in, out);
    printf("%s\n", out);
    return 0;
}

void expand(char s1[], char s2[])
{
    int c, i, j;
    for (i = 0, j = 0; i < strlen(s1); i++, j++) {
        s2[i] = s1[j];
    }
}
