#include "time.h"
#include <stdio.h>

int main(void)
{
    pcc_context_t *ctx = pcc_create(NULL);
    int result = 0;

    pcc_parse(ctx, &result);

    printf("Valid time.\n");

    pcc_destroy(ctx);
    return 0;
}
