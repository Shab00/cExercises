#include <stdio.h>
#include <ctype.h>

int my_htoi (char s[]);

int main()
{
    char s1[] = "1A";
    char s2[] = "0x1A";
    char s3[] = "FF";
    printf("my_htoi(\"%s\") = %d\n", s1, my_htoi(s1));
    printf("my_htoi(\"%s\") = %d\n", s2, my_htoi(s2));
    printf("my_htoi(\"%s\") = %d\n", s3, my_htoi(s3));
    return 0;
}

int my_htoi (char s[])
{
    int i = 0, n = 0;
    if (s[i] == '0') {
        ++i;
        if (s[i] == 'x' || s[i] == 'X') {
            ++i;
        }
    }

    for (; s[i] != '\0'; ++i) {
        int digit;
        if (s[i] >= '0' && s[i] <= '9') {
            digit = s[i] - '0';
        } else if (s[i] >= 'a' && s[i] <= 'f') {
            digit = s[i] - 'a' + 10;
        } else if (s[i] >= 'A' && s[i] <= 'F') {
            digit = s[i] - 'A' + 10;
        } else {
            break;
        }

        n = 16 * n + digit;
    }
    return n;
}
