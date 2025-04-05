#include <stdio.h>

int sum (int num) {
    static int total = 0;
    total += num;
    return total;
   }

int main() {
    printf("%d\n",sum(55));
    printf("%d\n",sum(45));
    printf("%d\n",sum(50));
    return 0;
}
