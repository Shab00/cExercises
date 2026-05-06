#include <stdio.h>

void my_strcat(char *s, char *t);

int main()
{
    char s[100] = "Hello";
    char t[] = "World";
    my_strcat(s, t);
    printf("%s\n", s);
    return 0;
}

void my_strcat(char *s, char *t) 
{
    while (*s) {
        s++;
    }
    while ((*s++ = *t++)) {
        ;
    }
}
