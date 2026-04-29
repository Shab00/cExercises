#ifndef CALC_H
#define CALC_H

#define NUMBER '0'
#define MAXOP 100
#define MAXVAL 100
#define MAXLINE 1000

extern int sp;
extern double val[MAXVAL];
extern char line[MAXLINE];
extern int li;

void push(double);
double pop(void);
int getop(char s[]);
int my_getline(char s[], int lim);

#endif
