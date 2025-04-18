#include <stdio.h>
#include <stdlib.h>

int main() {
    int nrows, i, j;

    printf("Enter the number of rows for the triangle: ");
    scanf("%d", &nrows);

    int **array = (int **) malloc(nrows * sizeof(int *));

    for (i = 0; i < nrows; i++) {
        array[i] = (int *) malloc((i + 1) * sizeof(int));
        for (j = 0; j <= i; j++) {
            array[i][j] = j + 1; 
        }
    }

    for (i = 0; i < nrows; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n"); 
    }

    for (i = 0; i < nrows; i++) {
        free(array[i]); 
    }
    free(array);

    return 0;
}
