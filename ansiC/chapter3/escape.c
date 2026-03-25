#include <stdio.h>

void escape(char s[], const char t[]);

int main()
{
    char t[1000], s[2000];
    int c = 0;
    
    while (fgets(t, sizeof(t), stdin)) {
        escape(s, t);
        printf("%s\n", s);
    }
    return 0;
}

void escape(char s[], const char t[])
{
    int i, j;
    for (i = 0, j = 0; t[i] != '\0'; i++) {
        switch(t[i]) {
            case '\n':
                s[j++] = '\\';
                s[j++] = 'n';
                break;
            case '\t':
                s[j++] = '\\';
                s[j++] = 't';
                break;
            default:
                s[j++] = t[i];
                break;
        }
    }
    s[j] = '\0';
}
