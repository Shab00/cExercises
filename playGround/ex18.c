#include <stdio.h>

int factorial(int a);

int main() {
    /* testing code */
    printf("0! = %i\n", factorial(0));
    printf("1! = %i\n", factorial(1));
    printf("3! = %i\n", factorial(3));
    printf("5! = %i\n", factorial(5));
}

/* define your function here (don't forget to declare it) */
int factorial(int a) {
    if (a == 0) {

        return 1;

    } else if (a >= 1) {

        return a * factorial(a - 1);

    }

    return 0;
}
