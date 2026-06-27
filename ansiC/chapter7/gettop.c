#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define NUMBER '0'

int getop(char s[])
{
    int c, rc;
    float f;

    while ((rc = scanf("%c", &c)) != EOF) {
        if ((s[0] = c) != ' ' && c != '\t') {
            break;
        }
    }
    s[1] = '\0';
    if (rc == EOF) {
        return EOF;
    } else if (!isdigit(c) && c != '.') {
        return c;
    }
    ungetc(c, stdin);
    scanf("%f", &f);
    sprintf(s, "%f", f);
    return NUMBER;
}

int main(void)
{
    char s[100];
    int type;

    while ((type = getop(s)) != EOF) {
        if (type == NUMBER) {
            printf("Number: %s\n", s);
        } else {
            printf("Operator: %c\n", type);
        }
    }
    return 0;
}
