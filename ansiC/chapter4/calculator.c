#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXOP 100
#define BUFSIZE 100
#define NUMBER '0'
#define MAXVAL 100
#define MAXLINE 1000

int getop(char[]);
int sp = 0;
int var = 0;
double v = 0;
void push(double);
double pop(void);
double last_printed = 0.0;
double var_array[26];
double val[MAXVAL];
int my_getline(char s[], int lim);
int buf = 0;
int li = 0;
char line[MAXLINE];
char recentVar;

int main()
{
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

void push(double f) 
{
    if (sp < MAXVAL) {
        val[sp++] = f;
    } else {
        printf("error: stack full, can't push %g\n", f);
    }
}

double pop(void) {
    if (sp > 0) {
        return val[--sp];
    } else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

int getop(char s[]) 
{
    int i, c;
    while (line[li] == '\0') {
        if (my_getline(line, MAXLINE) == 0)
            return EOF;
        li = 0;
    }

    while ((s[0] = c = line[li++]) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    if (!isdigit(c) && c != '.') {
        return c;
    }
    i = 0;
    if (isdigit(c))
        while (isdigit(s[++i] = c = line[li++]))
            ;

    if (c == '.')
        while (isdigit(s[++i] = c = line[li++]))
            ;

    s[i] = '\0';

    if (c != '\0')
        li--;

    return NUMBER;
}

int my_getline(char s[], int lim)
{
    int c, i, j;
    j = 0;    
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
        if (i < lim - 2) {
            s[i] = c;
            ++j;
        }
    }

    if (c == '\n') {
        s[j] = c;
        ++j;
        ++i;
    }
    s[j] = '\0';
    return i;
}
