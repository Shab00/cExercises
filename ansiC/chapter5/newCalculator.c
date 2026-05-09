#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

void push(double f) {
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

double pop(void) {
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

int main(int argc, char *argv[]) {
    double op2;

    while (--argc > 0) {
        if (sscanf(*++argv, "%lf", &op2) == 1) {
            push(op2);
        } else if (*argv[0] == '+' && argv[0][1] == '\0') {
            push(pop() + pop());
        } else if (*argv[0] == '*' && argv[0][1] == '\0') {
            push(pop() * pop());
        } else if (*argv[0] == '-' && argv[0][1] == '\0') {
            op2 = pop();
            push(pop() - op2);
        } else if (*argv[0] == '/' && argv[0][1] == '\0') {
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
        } else if (*argv[0] == '%' && argv[0][1] == '\0') {
            op2 = pop();
            if (op2 != 0.0)
                push(fmod(pop(), op2));
            else
                printf("error: zero divisor\n");
        } else {
            printf("error: unknown command %s\n", *argv);
            argc = 1;
        }
    }
    printf("\t%.8g\n", pop());
    return 0;
}
