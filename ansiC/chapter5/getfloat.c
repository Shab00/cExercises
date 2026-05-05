#include <stdio.h>
#include <ctype.h>

#define BUFSIZE 100

static int buf[BUFSIZE];
static int bufp = 0;

int getch(void);
void ungetch(int);
int getfloat(float *pn);

int main()
{
    int c;
    float n;

    while ((c = getfloat(&n)) != EOF) {
        if (c == 0) {
            c = getch();
            if (c == EOF) break;
            printf("non-number char: '%c'\n", c);
        } else if (c == '+' || c == '-'){
            printf("non-number char: '%c'\n", c);
            (void)getch();
        } else {
            printf("got float: %f\n", n);
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

int getfloat(float *pn) 
{
    int c, d, sign, temp;
    float power;

    while (isspace(c = getch())) {
        ;
    }

    if (!isdigit(c) && c != EOF && c != '+' && c != '-' && c != '.') {
        ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;

    if (c == '+' || c == '-') {
        temp = c;
        d = getch();
        if (!isdigit(d) && d != '.' && d != EOF) {
            ungetch(d);
            ungetch(temp);
            return temp;
        } 
        c = d;
    }
    

    *pn = 0.0;
    power = 1.0;

    while (isdigit(c)) {
        *pn = 10.0 * *pn + (c - '0');
        c = getch();
    }

    if (c == '.') {
        c = getch();
        while (isdigit(c)) {
            *pn = 10.0 * *pn + (c - '0');
            power *= 10.0;
            c = getch();
        }
    }

    *pn = sign * (*pn / power);
    if (c != EOF) {
        ungetch(c);
    }

    return c;
}
