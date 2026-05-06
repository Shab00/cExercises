#include <stdio.h>

int my_strend(char *s, char *t);

int main()
{
    char s[100] = "Hello";
    char t[100] = "ll";
    printf("%d\n", my_strend(s, t));
    return 0;
}

int my_strend(char *s, char *t)
{
    char *s_end = s, *t_end = t;

    while (*s_end) {
        s_end++;
    } 
    while (*t_end) {
        t_end++;
    }

    int len_s = s_end - s;
    int len_t = t_end - t;

    if (len_t > len_s) {
        return 0;
    }

    s_end -= len_t;
    while (*t) {
        if (*s_end++ != *t++) {
            return 0;
        }
    }
    return 1;
}
