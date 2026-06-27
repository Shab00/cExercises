#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    int c;
    char *progname;

    progname = argv[0];
    if (strstr(progname, "lower") != NULL) {
        while ((c = getchar()) != EOF) {
            putchar(tolower(c));
        }
    } else if (strstr(progname, "upper") != NULL) {
        while ((c = getchar()) != EOF) {
            putchar(toupper(c));
        }
    } else {
        printf("Usage: invoke as 'lower' or 'upper'\n");
        return 1;
    }
    return 0;
}
