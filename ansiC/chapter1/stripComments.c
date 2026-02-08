#include <stdio.h>

int main()
{
    int c, comment;
    c = comment = 0;
    while((c = getchar()) != EOF) {
        if (c == '/') {
            continue;
        } else if (c == '*') {
            comment++;
            continue;
        } 
    }

    printf("%d\n", comment);
    return 0;
}
