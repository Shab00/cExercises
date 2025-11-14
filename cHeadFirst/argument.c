#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

void error_exit(const char *msg, int errnum)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errnum));
    exit(1);
}

void *does_not(void *arg)
{
    (void)arg;
    printf("does_not: starting\n");
    sleep(1);
    printf("does_not: finishing\n");

    int *result = malloc(sizeof(int));
    if (!result) return NULL;
    *result = 100;
    return result;
}

void *does_too(void *arg)
{
    (void)arg;
    printf("does_too: starting\n");
    sleep(2);
    printf("does_too: finishing\n");

    int *result = malloc(sizeof(int));
    if (!result) return NULL;
    *result = 200;
    return result;
}

int main(void)
{
    pthread_t t0, t1;
    int rc;

    rc = pthread_create(&t0, NULL, does_not, NULL);
    if (rc != 0) error_exit("Can't create thread t0", rc);

    rc = pthread_create(&t1, NULL, does_too, NULL);
    if (rc != 0) error_exit("Can't create thread t1", rc);

    void *res0;
    rc = pthread_join(t0, &res0);
    if (rc != 0) error_exit("Can't join thread t0", rc);
    if (res0) {
        printf("t0 returned %d\n", *(int *)res0);
        free(res0);
    } else {
        printf("t0 returned NULL\n");
    }

    void *res1;
    rc = pthread_join(t1, &res1);
    if (rc != 0) error_exit("Can't join thread t1", rc);
    if (res1) {
        printf("t1 returned %d\n", *(int *)res1);
        free(res1);
    } else {
        printf("t1 returned NULL\n");
    }

    printf("Both threads finished; main exiting.\n");
    return 0;
}
