#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#define LOCALFMT 100

void minscanf(char *fmt, ...)
{
    va_list ap;
    char *p, *sval;
    char localfmt[LOCALFMT];
    int c, i, *ival;
    unsigned *uval;
    double *dval;

    i = 0;
    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            localfmt[i++] = *p;
            continue;
        }
        localfmt[i++] = '%';
        while (*(p + 1) && !isalpha(*(p + 1))) {
            localfmt[i++] = *++p;
        }
        localfmt[i++] = *(p + 1);
        localfmt[i] = '\0';
        switch (*++p) {
        case 'd':
        case 'i':
            ival = va_arg(ap, int *);
            scanf(localfmt, ival);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            uval = va_arg(ap, unsigned *);
            scanf(localfmt, uval);
            break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            dval = va_arg(ap, double *);
            scanf(localfmt, dval);
            break;
        case 'c':
            sval = va_arg(ap, char *);
            scanf(localfmt, sval);
            break;
        case 's':
            sval = va_arg(ap, char *);
            scanf(localfmt, sval);
            break;
        default:
            scanf(localfmt);
            break;
        }
        i = 0;
    }
    va_end(ap);
}

int main(void)
{
    int day, year;
    char month[20];

    printf("Enter date (dd month yyyy): ");
    minscanf("%d %s %d", &day, month, &year);
    printf("You entered: %d %s %d\n", day, month, year);
    return 0;
}
