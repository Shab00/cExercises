#include <stdio.h>
#include <stdlib.h>

int main() {
    int nrows = 6;
    int i, j;

    char **pdashes;
    
    pdashes = (char **) malloc(nrows * sizeof(char *));

    for (i = 0; i < nrows; i++) {
        pdashes[i] = (char *) malloc((i + 1) * sizeof(char));
    } 
    pdashes[0][0] = '-';

    pdashes[1][0] = '-';
    pdashes[1][1] = '-';

    pdashes[2][0] = '-';
    pdashes[2][1] = '-';
    pdashes[2][2] = '-';

    pdashes[3][0] = '-';
    pdashes[3][1] = '-';
    pdashes[3][2] = '-';
    pdashes[3][3] = '-';

    pdashes[4][0] = '-';
    pdashes[4][1] = '-';
    pdashes[4][2] = '-';
    pdashes[4][3] = '-';
    pdashes[4][4] = '-';

    pdashes[5][0] = '-';
    pdashes[5][1] = '-';
    pdashes[5][2] = '-';
    pdashes[5][3] = '-';
    pdashes[5][4] = '-';
    pdashes[5][5] = '-';

    for (i = 0; i < nrows; i++) {
        for (j = 0; j <= i; j++) {
            printf("%c ", pdashes[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < nrows; i++) {
        free(pdashes[i]);
    }
    free(pdashes);
}
