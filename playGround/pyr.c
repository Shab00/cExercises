#include <stdio.h>

int main() {
    int height = 5;
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}
