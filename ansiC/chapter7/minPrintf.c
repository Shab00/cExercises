#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#define LOCALFMT 100

void minprintf(char *fmt, ...)
{
    va_list ap;
    char *p, *sval;
    char localfmt[LOCALFMT];
    int i, ival;
    unsigned uval;
    double dval;

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        i = 0;
        localfmt[i++] = '%';
        while (*(p + 1) && !isalpha(*(p + 1))) {
            localfmt[i++] = *++p;
        }
        localfmt[i++] = *(p + 1);
        localfmt[i] = '\0';
        switch (*++p) {
        case 'd':
        case 'i':
            ival = va_arg(ap, int);
            printf(localfmt, ival);
            break;
        case 'x':
        case 'X':
        case 'u':
        case 'o':
            uval = va_arg(ap, unsigned);
            printf(localfmt, uval);
            break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            dval = va_arg(ap, double);
            printf(localfmt, dval);
            break;
        case 'c':
            ival = va_arg(ap, int);
            printf(localfmt, ival);
            break;
        case 's':
            sval = va_arg(ap, char *);
            printf(localfmt, sval);
            break;
        case 'p':
            sval = va_arg(ap, void *);
            printf(localfmt, sval);
            break;
        case '%':
            putchar('%');
            break;
        default:
            printf(localfmt);
            break;
        }
    }
    va_end(ap);
}

int main(void)
{
    minprintf("Integer: %d\n", 42);
    minprintf("Hex: %x\n", 255);
    minprintf("Float: %10.2f\n", 3.14159);
    minprintf("String: %-20s end\n", "hello");
    minprintf("Percent: 100%%\n");
    return 0;
}
