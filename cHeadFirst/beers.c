#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int beers = 2000000;

void* drink_lots(void *arg)
{
    (void)arg;
    for (int i = 0; i < 100000; i++) {
        beers = beers - 1;
    }
    return NULL;
}

int main(void)
{
    pthread_t threads[20];

    printf("%d bottles of beer on the wall\n%d bottles of beer\n", beers, beers);

    for (int t = 0; t < 20; t++) {
        int rc = pthread_create(&threads[t], NULL, drink_lots, NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_create failed: %d\n", rc);
            exit(1);
        }
    }

    for (int t = 0; t < 20; t++) {
        int rc = pthread_join(threads[t], NULL);
        if (rc != 0) {
            fprintf(stderr, "pthread_join failed: %d\n", rc);
            exit(1);
        }
    }

    printf("There are now %d bottles of beer on the wall\n", beers);
    return 0;
}
