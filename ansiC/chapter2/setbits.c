#include <stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y);

int main()
{
    unsigned x = 0xF0;   /* 1111 0000 */
    unsigned y = 0x0A;   /* 0000 1010 */
    int p = 5, n = 3;

    unsigned r = setbits(x, p, n, y);
    printf("x=0x%X, y=0x%X, p=%d, n=%d -> result=0x%X (%u)\n", x, y, p, n, r, r);
    return 0;
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
    if (n <= 0) return x;
    unsigned mask = (1u << n) - 1u;
    unsigned shift = (unsigned)(p + 1 - n);
    unsigned y_bits = (y & mask) << shift;
    unsigned clear_mask = ~(mask << shift);
    return (x & clear_mask) | y_bits;
}
