#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

static int buf[BUFSIZE];
static int bufp = 0;

int getch(void);
void ungetch(int);
int getint(int *pn);

int main()
{
    int n, c;

    while ((c = getint(&n)) != EOF) {
        if (c == 0) {
            c = getch();
            if (c == EOF) break;
            printf("non-number char: '%c'\n", c);
        } else if (c == '+' || c == '-'){
            printf("non-number char: '%c'\n", c);
            (void)getch();
        } else {
            printf("got int: %d\n", n);
        }
    }
    return 0;
}

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }

}

int getint(int *pn) 
{
    int c, d, sign, temp;

    while (isspace(c = getch())) {
        ;
    }

    if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-') {
        temp = c;
        d = getch();
        if (!isdigit(d) && d != EOF) {
            ungetch(d);
            ungetch(temp);
            return temp;
        } 
        c = d;
    }

    for (*pn = 0; isdigit(c); c = getch()) {
        *pn = 10 * *pn + (c - '0');
    }

    *pn *= sign;

    if (c != EOF) {
        ungetch(c);
    }

    return c;
}
