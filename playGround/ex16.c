#include <stdio.h>
#include <stdlib.h>

int main() {
    int nrows = 3;
    int i, j;

    /* Define the 2D pointer variable here */
    int **pnumbers;

    /* Allocate memory for holding three rows */
    pnumbers = (int **) malloc(nrows * sizeof(int *));

    /* Allocate memory for storing the individual elements in each row */
    for (i = 0; i < nrows; i++) {
        pnumbers[i] = (int *) malloc((i + 1) * sizeof(int)); // Allocate enough space for i + 1 elements
    }

    /* Assign values to the array */
    pnumbers[0][0] = 1;
    pnumbers[1][0] = 1;
    pnumbers[1][1] = 1;
    pnumbers[2][0] = 1;
    pnumbers[2][1] = 2;
    pnumbers[2][2] = 1;

    /* Print the triangular array */
    for (i = 0; i < nrows; i++) {
        for (j = 0; j <= i; j++) {
            printf("%d ", pnumbers[i][j]);
        }
        printf("\n");
    }

    /* Free memory allocated for each row */
    for (i = 0; i < nrows; i++) {
        free(pnumbers[i]);
    }

    /* Free the top-level pointer */
    free(pnumbers);

    return 0;
}
