#include <stdio.h>

int bitcount(unsigned x);

int main()
{
    unsigned x = 0xF0;   /* 1111 0000 */
    unsigned r = bitcount(x);
    printf("x=0x%X -> result=0x%X (%u)\n", x, r, r);
    return 0;
}

int bitcount(unsigned x)
{
    int b = 0;
    
    while(x != 0) {
        x &= (x - 1);
        b++;
    }
    return b;
}
