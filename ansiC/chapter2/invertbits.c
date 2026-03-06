#include <stdio.h>

unsigned invertbits(unsigned x, int p, int n);

int main()
{
    unsigned x = 0xF0;   /* 1111 0000 */
    int p = 1, n = 3;

    unsigned r = invertbits(x, p, n);
    printf("x=0x%X, p=%d, n=%d -> result=0x%X (%u)\n", x, p, n, r, r);
    return 0;
}
unsigned invertbits(unsigned x, int p, int n)
{
    if (n <= 0 || p < 0) return x;

    int w = (int)(sizeof(unsigned) * 8);
    if (p >= w) return x;

    if (n > p + 1) n = p + 1;

    unsigned ones = (n >= w) ? ~0u : ((1u << n) - 1u);

    unsigned mask = ones << (p + 1 - n);

    return x ^ mask;
}
