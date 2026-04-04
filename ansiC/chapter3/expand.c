#include <stdio.h>
#include <string.h>

struct TestCase {
    const char *input;
    const char *expected;
};

struct TestCase testcases[] = {
    {"a-z", "abcdefghijklmnopqrstuvwxyz"},
    {"0-3", "0123"},
    {"a-d0-2", "abcd012"},
};

int valid_range(char left, char right); 
void expand(const char s1[], char s2[]);

int main()
{
    char output[100];
    int ncases = sizeof(testcases)/sizeof(testcases[0]);
    for (int i = 0; i < ncases; ++i) {
        expand(testcases[i].input, output);
        if (strcmp(output, testcases[i].expected) == 0)
            printf("PASS: \"%s\" -> \"%s\"\n", testcases[i].input, output);
        else
            printf("FAIL: \"%s\" -> \"%s\" (expected \"%s\")\n",
                   testcases[i].input, output, testcases[i].expected);
    }
    return 0;
}

int valid_range(char left, char right) {
    return (
        (left >= 'a' && left <= 'z' && right >= 'a' && right <= 'z') ||
        (left >= 'A' && left <= 'Z' && right >= 'A' && right <= 'Z') ||
        (left >= '0' && left <= '9' && right >= '0' && right <= '9')
    ) && (left < right);
}

void expand(const char s1[], char s2[])
{
    int i, j, c;
    for (i = 0, j = 0; s1[i] != '\0'; i++) {
        if (s1[i] == '-' && i > 0 && s1[i+1] != '\0' && valid_range(s1[i-1], s1[i+1])) {
            for (char c = s1[i-1] + 1; c < s1[i+1]; c++) {
                s2[j++] = c;
            }
        } else {
            s2[j++] = s1[i];
        }
    }
    s2[j] = '\0';
}
