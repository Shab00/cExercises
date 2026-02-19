#include <stdio.h>

void squeeze(char s[], int c);

int main()
{
    int c = 'i';
    char s[1000];
    while (fgets(s, sizeof(s), stdin) != NULL) {
        printf("Before: %s\n", s);
        squeeze(s, c);
        printf("After: %s\n", s);
    }
    return 0; 
}

void squeeze(char s[], int c) 
{
    int i, j;

    for (i = j = 0; s[i] != '\0'; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';

}
