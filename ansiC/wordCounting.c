#include <stdio.h>

int main(){
    int c, nl, b, t;
    
    c = nl = b = t = 0;
    while ((c = getchar()) != EOF)
        if (c == '\n')
            ++nl;
    printf("%d\n", nl);
    return 0;
}
