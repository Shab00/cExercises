#include <stdio.h>
#include <string.h>

struct TestCase {
    const int input;
    const int base;
    const char *expected;
};

struct TestCase testcases[] = {
    {0, 2, "0"},
    {1, 2, "1"},
    {5, 2, "101"},
    {15, 2, "1111"},
    {0, 10, "0"},
    {123, 10, "123"},
    {-123, 10, "-123"},
    {63, 8, "77"},
    {255, 16, "ff"},
    {48879, 16, "beef"},
    {-255, 16, "-ff"},
    {31, 16, "1f"},
    {31, 8, "37"},
    {31, 2, "11111"},
    {12345, 36, "9ix"}
};

void itob(int n, char s[], int b);
void reverse(char s[]);

int main()
{
    char output[100];
    int ncases = sizeof(testcases)/sizeof(testcases[0]);
    for (int i = 0; i < ncases; ++i) {
        itob(testcases[i].input, output, testcases[i].base);
        if (strcmp(output, testcases[i].expected) == 0)
            printf("PASS: %d (base %d) -> \"%s\"\n", testcases[i].input, testcases[i].base, output);
        else
            printf("FAIL: %d (base %d) -> \"%s\" (expected \"%s\")\n",
                   testcases[i].input, testcases[i].base, output, testcases[i].expected);
    }
    return 0;
}

void itob(int n, char s[], int b)
{
    int digit, i = 0, sign = n, temp = n;
    if (temp == 0) {
        s[i++] = '0';
    } else {
        while (temp != 0) {
            digit = temp % b;
            if (digit < 0) digit = -digit;
            s[i++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
            temp /= b;
        }

        if (sign < 0) {
            s[i++] = '-';
        }

    }
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[]) 
{
    int c, i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }

}
