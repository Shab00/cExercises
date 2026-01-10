#include <stdio.h>

int main(){
    int c, nl, b, t;
    
    c = nl = b = t = 0;
    while ((c = getchar()) != EOF)
        if (c == '\n')
            ++nl;
        else if (c == '\t')
            ++t;
        else if (c == ' ')
            ++b;
    printf("The amount of lines: %d\n", nl);
    printf("The amount of tabs: %d\n", t);
    printf("The amount of spaces: %d\n", b);
    return 0;
}
