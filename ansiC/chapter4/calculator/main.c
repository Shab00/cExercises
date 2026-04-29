#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "calc.h"

int sp = 0;
double val[MAXVAL];
char line[MAXLINE];
int li = 0;

int var = 0;
double v = 0;
double last_printed = 0.0;
double var_array[26];
char recentVar;

int main() {
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case 'p':
                printf("%.8g\n", val[sp-1]);
                break;
            case 'd':
                if (sp > 0) {
                    push(val[sp-1]);
                } else {
                    printf("error: stack empty, cannot duplicate\n");
                }
                break;
            case 'w':
                if (sp > 1) {
                    double temp = val[sp-1];
                    val[sp-1] = val[sp-2];
                    val[sp-2] = temp;
                } else {
                    printf("error: less than 2 numbers, cannot swap\n");
                }
                break;
            case 'x':
                sp = 0;
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0.0){
                    push(pop() / op2);
                } else {
                    printf("error: zero divisor\n");
                }
                break;
            case '%':
                op2 = pop();
                if (op2 != 0.0){
                    push((int)pop() % (int)op2);
                } else {
                    printf("error: zero divisor\n");
                }
                break;
            case 's':
                push(sin(pop()));
                break;
            case 'c':
                push(cos(pop()));
                break;
            case '^':
                push(pow(pop(), pop()));
                break;
            case '=':
                pop();
                if (var >= 'A' && var <= 'Z')
                    var_array[var - 'A'] = pop();
                else
                    printf("error: no variable name\n");
                break;
            case '\n':
                v = pop();
                printf("\t%.8g\n", v);
                break;
            default:
                if (type >= 'A' && type <= 'Z') {
                    push(var_array[type - 'A']);
                } else if (type == 'v') {
                    push(v);
                } else {
                    printf("error: unknown command %s\n", s);
                }
                break;
        }
        var = type;
    }
    return 0;
}
