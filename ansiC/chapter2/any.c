#include <stdio.h>

int any(char s[], char t[]);

int main()
{
    char s[1000];
    int result;
    char t[] = "aeiou";
    while (fgets(s, sizeof(s), stdin) != NULL) {
        result = any(s, t);
        printf("After: %d\n", result);
    }
    return 0; 
}

int any(char s[], char t[]) 
{
    int i, j;
    for (i = 0; s[i] != '\0'; i++) {
        for (j = 0; t[j] != '\0'; j++) {
            if (s[i] == t[j]) {
                return i;
            }
        }

    }
    return -1;
}
