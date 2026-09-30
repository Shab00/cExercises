#include "turtle.h"
#include <stdio.h>

void turtle_init(struct turtle *t) {
   t->x = 20;
   t->y = 10;
   t->pen_number = 1;
   t->pen_down = false;

   for (int i = 0; i < GRID_HEIGHT; i++) {
       for (int j = 0; j < GRID_WIDTH; j++) {
           t->grid[i][j] = '.';
       }
   }
}

static const char PEN_SYMBOLS[] = {'-', '#', '*'};

static void move_turtle(struct turtle *t, int dx, int dy, int steps) {
    for (int i = 0; i < steps; i++) {
        int next_x = t->x + dx;
        int next_y = t->y + dy;

        if (next_x < 0 || next_x >= GRID_WIDTH || next_y < 0 || next_y >= GRID_HEIGHT) {
            break;
        }

        t->x = next_x;
        t->y = next_y;

        if (t->pen_down) {
            t->grid[t->y][t->x] = PEN_SYMBOLS[t->pen_number - 1];
        }
    }
}

static bool parse_line(const char *line, char *cmd_out, int *num_out) {
    int i = 0;
    for (; line[i] != '\0' && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'); i++) {

    }

    if (line[i] == '\0' || line[i] == '#') {
        return false;
    }     

    *cmd_out = line[i];
    i++;

    for (; line[i] != '\0' && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'); i++) {

    }
    
    *num_out = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        *num_out = *num_out * 10 + (line[i] - '0');
        i++;
    }

    return true;

}

void turtle_print(const struct turtle *t) {

   for (int i = 0; i < GRID_HEIGHT; i++) {
       for (int j = 0; j < GRID_WIDTH; j++) {
           printf("%c", t->grid[i][j]);
       }
       printf("\n");
   }
}


void turtle_execute(struct turtle *t, const char *line) {
    char cmd;
    int num;

    if (!parse_line(line, &cmd, &num)) {
        return;
    }
    switch (cmd) {
        case 'D':
            t->pen_down = true;
            return;
        case 'U':
            t->pen_down = false;
            return;
        case 'P':
            if (num < 1 || num > 3) {
                fprintf(stderr, "Error: Pen number %d is out of bounds (must be 1-3).\n", num);
                return;
            }
            t->pen_number = num;
            return;
        case 'N':
            move_turtle(t, 0, -1, num);
            return;
        case 'S':
            move_turtle(t, 0, +1, num);
            return;
        case 'W':
            move_turtle(t, -1, 0, num);
            return;
        case 'E':
            move_turtle(t, +1, 0, num);
            return;
        default:
            fprintf(stderr, "unknown command: %c\n", cmd);
            return;
    }

}

void turtle_pen(struct turtle *t, int n) {
    if (n < 1 || n > 3) {
        fprintf(stderr, "Error: Pen number %d is out of bounds (must be 1-3).\n", n);
        return;
    }
    t->pen_number = n;
}

void turtle_up(struct turtle *t) {
    t->pen_down = false;
}

void turtle_down(struct turtle *t) {
    t->pen_down = true;
}

void turtle_north(struct turtle *t, int n) {
    move_turtle(t, 0, -1, n);
}

void turtle_south(struct turtle *t, int n) {
    move_turtle(t, 0, +1, n);
}

void turtle_east(struct turtle *t, int n) {
    move_turtle(t, 1, 0, n);
}

void turtle_west(struct turtle *t, int n) {
    move_turtle(t, -1, 0, n);
}
