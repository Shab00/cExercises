#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define MAXLINE 100

double extend(char s[]);
int my_getline(char s[], int lim);

int main()
{
    double sum = 0;
    char line[MAXLINE];
    while (my_getline(line, MAXLINE) > 0) {
        if (line[0] == '\n' || line[0] == '\0')
            continue; // Skip blank/empty lines
        double val = extend(line);
        sum += val;
        printf("result => %f, sum => %f, original => %s\n", val, sum, line);
    }
    return 0;
}

int my_getline(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

double extend(char s[]) 
{
    double val, power;
    int i, sign, exp_sign, exponent;

    for (i = 0; isspace(s[i]); i++)
        ;

    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '-' || s[i] == '+')
        i++;

    val = 0.0;
    while (isdigit(s[i]))
        val = 10.0 * val + (s[i++] - '0');

    power = 1.0;
    if (s[i] == '.') {
        i++;
        while (isdigit(s[i])) {
            val = 10.0 * val + (s[i++] - '0');
            power *= 10.0;
        }
    }

    double result = sign * val / power;

    if (s[i] == 'e' || s[i] == 'E') {
        i++;
        exp_sign = 1;
        if (s[i] == '-') {
            exp_sign = -1;
            i++;
        } else if (s[i] == '+') {
            i++;
        }
        exponent = 0;
        while (isdigit(s[i])) {
            exponent = 10 * exponent + (s[i++] - '0');
        }
        exponent *= exp_sign;
        result *= pow(10.0, exponent);
    }

    return result;
}
