#include <stdio.h>

int main() {
    int i;
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int sum = 0;
    unsigned long long factorial = 1;

    for (i = 0; i < 10; i++) {
        printf("%d\n", i);
    }

    for (i = 0; i < 10; i++) {
        sum += array[i];
    }

    for (i = 0; i < 10; i++) {
        factorial *= array[i];
    }

    printf("Sum of the array is %d\n", sum);
    printf("10! is %llu\n", factorial);

    return 0;
}
