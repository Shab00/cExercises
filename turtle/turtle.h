#ifndef TURTLE_H
#define TURTLE_H

#include <stdbool.h>

#define GRID_HEIGHT 20
#define GRID_WIDTH  40

struct turtle {
    int x;
    int y;
    int pen_number;
    char grid[GRID_HEIGHT][GRID_WIDTH];
    bool pen_down;
};

void turtle_init(struct turtle *t);

void turtle_print(const struct turtle *t);

void turtle_execute(struct turtle *t, const char *line);

void turtle_pen(struct turtle *t, int n);

void turtle_up(struct turtle *t);

void turtle_down(struct turtle *t);

void turtle_north(struct turtle *t, int n);

void turtle_south(struct turtle *t, int n);

void turtle_east(struct turtle *t, int n);

void turtle_west(struct turtle *t, int n);

#endif
