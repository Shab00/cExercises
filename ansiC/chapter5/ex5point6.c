#include <stdio.h>
#include <string.h>

int atoi(char s[]); 
void reverse(char s[]); 

int main()
{
    char tests[][20] = {"12345", "42abc", "   77", "", "foo", "abcde", "racecar", "z", "Hi there!"};
    int i;
    printf("Testing atoi:\n");
    for (i = 0; i < 5; i++) {
        printf("String: '%s', atoi: %d\n", tests[i], atoi(tests[i]));
    }

    printf("\nTesting reverse:\n");
    for (i = 5; i < 9; i++) {
        char buf[20];
        strcpy(buf, tests[i]);
        reverse(buf);
        printf("Original: '%s', reversed: '%s'\n", tests[i], buf);
    }

    return 0;
}

void reverse(char *s) 
{
    char *l = s;
    char *r = s + strlen(s) - 1;
    char temp;

    while (l < r) {
        temp = *l;
        *l = *r;
        *r = temp;
        l++;
        r--;
    }
}

int atoi(char *s) 
{
    int n = 0; 
    while (*s) {
        if (*s >= '0' && *s <= '9') {
            n = 10 * n + (*s - '0');
        }
        s++;
    }
    return n;
}
