#include "turtle.h"
#include <stdio.h>

int main(void)
{
    struct turtle external;
    struct turtle internal;

    // ---------- External DSL: strings + parser ----------
    turtle_init(&external);

    turtle_execute(&external, "P 2");
    turtle_execute(&external, "D");
    turtle_execute(&external, "W 2");
    turtle_execute(&external, "N 3");
    turtle_execute(&external, "E 5");
    turtle_execute(&external, "S 4");
    turtle_execute(&external, "W 4");
    turtle_execute(&external, "N 4");
    turtle_execute(&external, "E 3");
    turtle_execute(&external, "S 2");
    turtle_execute(&external, "W 1");

    printf("External DSL (strings + parser):\n");
    turtle_print(&external);

    // ---------- Internal DSL: function calls ----------
    turtle_init(&internal);

    turtle_pen(&internal, 2);
    turtle_down(&internal);
    turtle_west(&internal, 2);
    turtle_north(&internal, 3);
    turtle_east(&internal, 5);
    turtle_south(&internal, 4);
    turtle_west(&internal, 4);
    turtle_north(&internal, 4);
    turtle_east(&internal, 3);
    turtle_south(&internal, 2);
    turtle_west(&internal, 1);

    printf("\nInternal DSL (function calls):\n");
    turtle_print(&internal);

    return 0;
}
