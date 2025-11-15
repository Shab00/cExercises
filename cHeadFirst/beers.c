#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 20
#define ITER_PER_THREAD 100000

int beers = 2000000;
pthread_mutex_t beers_lock = PTHREAD_MUTEX_INITIALIZER;

void *drink_lots_A(void *arg)
{
    int i;
    (void)arg;
    pthread_mutex_lock(&beers_lock);
    for (i = 0; i < ITER_PER_THREAD; i++) {
        beers = beers - 1;
    }
    pthread_mutex_unlock(&beers_lock);

    printf("beers = %i\n", beers);
    return NULL;
}

void *drink_lots_B(void *arg)
{
    int i;
    (void)arg;
    for (i = 0; i < ITER_PER_THREAD; i++) {
        pthread_mutex_lock(&beers_lock);
        beers = beers - 1;
        pthread_mutex_unlock(&beers_lock);
    }

    printf("beers = %i\n", beers);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2 || (argv[1][0] != 'A' && argv[1][0] != 'B')) {
        fprintf(stderr, "Usage: %s A|B\n", argv[0]);
        fprintf(stderr, "  A = coarse-grained lock (lock once per thread loop)\n");
        fprintf(stderr, "  B = fine-grained lock (lock per decrement)\n");
        return 1;
    }

    printf("%d bottles of beer on the wall\n", beers);
    printf("%d bottles of beer\n", beers);

    pthread_t threads[N_THREADS];
    int i;
    void *(*fn)(void *) = (argv[1][0] == 'A') ? drink_lots_A : drink_lots_B;

    for (i = 0; i < N_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, fn, NULL) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("There are now %d bottles of beer on the wall\n", beers);
    return 0;
}
