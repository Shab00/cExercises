#include <stdio.h>
#include <time.h>

/* Version 1: Function - saves space */
int isupper_func(char c)
{
    if (c >= 'A' && c <= 'Z') {
        return 1;
    } else {
        return 0;
    }
}

/* Version 2: Macro - saves time */
#define isupper_macro(c) ((c) >= 'A' && (c) <= 'Z') ? 1 : 0

/* Version 3: Macro with protection against double evaluation */
#define isupper_safe(c) ({ \
    char _c = (c); \
    (_c >= 'A' && _c <= 'Z') ? 1 : 0; \
})

int main(void)
{
    char *p = "This is a Test String";
    char *q;

    /* Function version - safe, *p evaluated once */
    q = p;
    printf("Function version:\n");
    while (*q) {
        if (isupper_func(*q)) {
            printf("%c is upper\n", *q);
        }
        q++;
    }

    /* Macro version - UNSAFE with *p++ */
    q = p;
    printf("\nMacro version (safe usage):\n");
    while (*q) {
        if (isupper_macro(*q)) {
            printf("%c is upper\n", *q);
        }
        q++;
    }

    /* The danger of macros with side effects */
    q = p;
    printf("\nDANGER - macro with *q++:\n");
    printf("Characters checked: ");
    while (*q) {
        if (isupper_macro(*q++)) {
            printf("UPPER ");
        } else {
            printf("lower ");
        }
    }
    printf("\n(Notice: pointer incremented TWICE per character!)\n");

    return 0;
}
