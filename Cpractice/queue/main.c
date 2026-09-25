#include "queue.h"
#include <stdio.h>

int main(void)
{
    struct queue *q = queue_create();
    if (q == NULL) {
        printf("queue_create failed\n");
        return 1;
    }
    printf("queue created\n");

    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    queue_enqueue(q, 3);
    printf("count after 3 enqueues: %zu (expect 3)\n", queue_count(q));

    q_data_t value = 0;
    bool ok = queue_peek(q, &value);
    printf("peek: ok=%d value=%d (expect ok=1 value=1)\n", ok, value);
    printf("count after peek: %zu (expect 3)\n", queue_count(q));

    ok = queue_dequeue(q, &value);
    printf("dequeue 1: ok=%d value=%d (expect ok=1 value=1)\n", ok, value);

    ok = queue_dequeue(q, &value);
    printf("dequeue 2: ok=%d value=%d (expect ok=1 value=2)\n", ok, value);

    ok = queue_dequeue(q, &value);
    printf("dequeue 3: ok=%d value=%d (expect ok=1 value=3)\n", ok, value);

    ok = queue_dequeue(q, &value);
    printf("dequeue on empty: ok=%d (expect ok=0)\n", ok);

    ok = queue_peek(q, &value);
    printf("peek on empty: ok=%d (expect ok=0)\n", ok);

    printf("is_empty: %d (expect 1)\n", queue_is_empty(q));

    queue_destroy(q);

    return 0;
}
