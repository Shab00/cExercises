#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define MAXOUT   1000

enum { NAME, PARENS, BRACKETS };

int tokentype;
char token[MAXTOKEN];
char out[MAXOUT];

int gettoken(void);
int getch(void);
void ungetch(int);
int needsparens(const char *s);

int needsparens(const char *s)
{
    int len = strlen(s);

    if (len >= 2 && strcmp(s + len - 2, "()") == 0)
        return 1;
    if (len >= 1 && s[len - 1] == ']')
        return 1;
    return 0;
}

int gettoken(void)
{
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    while ((c = getch()) == ' ' || c == '\t')
        ;

    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            token[0] = '(';
            token[1] = '\0';
            return tokentype = '(';
        }
    } else if (c == '[') {
        *p++ = '[';
        while ((*p++ = getch()) != ']' && *(p - 1) != EOF)
            ;
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        *p++ = c;
        while (isalnum(c = getch()))
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else {
        token[0] = c;
        token[1] = '\0';
        return tokentype = c;
    }
}

#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp < BUFSIZE)
        buf[bufp++] = c;
    else
        printf("ungetch: too many characters\n");
}

int main(void)
{
    int type;
    char temp[MAXTOKEN];

    while (gettoken() != EOF) {
        strcpy(out, token);

        while ((type = gettoken()) != '\n' && type != EOF) {
            if (type == PARENS || type == BRACKETS) {
                strcat(out, token);
            } else if (type == '*') {
                if (needsparens(out))
                    sprintf(temp, "(*%s)", out);
                else
                    sprintf(temp, "*%s", out);
                strcpy(out, temp);
            } else if (type == NAME) {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            } else {
                printf("invalid input at %s\n", token);
            }
        }

        printf("%s\n", out);

        if (type == EOF)
            break;
    }

    return 0;
}
