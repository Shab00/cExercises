#include <stdio.h>

int binsearch(int x, int v[], int n);

int main()
{
    int x = 3;
    int n = 10;
    int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int result;
    result = binsearch(x, v, n);
    printf("%d\n", result);
    return 0;
}

int binsearch(int x, int v[], int n)
{
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = (high + low) / 2;

        if (v[mid] == x) {
            return mid;
        }

        if (v[mid] > x) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return -1;
}
