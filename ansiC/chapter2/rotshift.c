#include <stdio.h>
#include <limits.h> 
#include <stdint.h>
unsigned rotate_right(unsigned x, unsigned n)
{
    unsigned w = sizeof(x) * CHAR_BIT;
    if (w == 0) return x;
    n %= w;
    if (n == 0) return x;
    return (x >> n) | (x << (w - n));
}

unsigned rotate_left(unsigned x, unsigned n)
{
    unsigned w = sizeof(x) * CHAR_BIT;
    if (w == 0) return x;
    n %= w;
    if (n == 0) return x;
    return (x << n) | (x >> (w - n));
}
int main(void) {
    unsigned x = 0xF0;
    printf("rotate_right(0x%X,3) = 0x%X\n", x, rotate_right(x,3));
    printf("rotate_left (0x%X,3) = 0x%X\n", x, rotate_left(x,3));
    return 0;
}
