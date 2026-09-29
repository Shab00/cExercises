#include "turtle.h"
#include <stdio.h>

int main(void)
{
    struct turtle t;

    turtle_init(&t);

    turtle_execute(&t, "P 2");
    turtle_execute(&t, "D");
    turtle_execute(&t, "W 2");
    turtle_execute(&t, "N 3");
    turtle_execute(&t, "E 5");
    turtle_execute(&t, "S 4");
    turtle_execute(&t, "W 4");
    turtle_execute(&t, "N 4");
    turtle_execute(&t, "E 3");
    turtle_execute(&t, "S 2");
    turtle_execute(&t, "W 1");

    turtle_print(&t);

    return 0;
}
