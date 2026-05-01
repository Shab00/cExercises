#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void itoa(int n, char s[]);
void reverse(char s[]); 
void reverser(char s[], int i, int len); 

int main()
{
    char s[10];
    int num = 13;
    int i;
    itoa(num, s);
    printf("%s\n", s);
    reverse(s);
    printf("%s\n", s);
    return 0;
}

void itoa(int n, char s[]) {
    static int i;
    if (n/10) {
        itoa(n/10, s);
    } else {
        i = 0;
        if (n < 0) {
            s[i++] = '-';
        }
    }
    s[i++] = abs(n) % 10 + '0';
    s[i] = '\0';
}

void reverser(char s[], int i, int len) {
    int c, j;
    j = len - (i + 1);
    if (i < j) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
        reverser(s, ++i, len);
    }
}

void reverse(char s[]) {
    reverser(s, 0, strlen(s));
}
