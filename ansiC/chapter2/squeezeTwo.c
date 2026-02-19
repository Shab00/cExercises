#include <stdio.h>

void squeeze(char s[], char t[]);

int main()
{
    char t[1000];
    char s[] = "aeiou";
    while (fgets(t, sizeof(t), stdin) != NULL) {
        printf("Before: %s\n", t);
        squeeze(t, s);
        printf("After: %s\n", t);
    }
    return 0; 
}

void squeeze(char t[], char s[]) 
{
    int i, j, l;
    int found;
    for (i = l = 0; t[i] != '\0'; i++) {
        found = 0;
        for (j = 0; s[j] != '\0'; j++) {
            if (t[i] == s[j]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            t[l++] = t[i];
        }
    }
    t[l] = '\0';
}
