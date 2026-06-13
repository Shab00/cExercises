#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define MAXOUT   1000

enum { NAME, PARENS, BRACKETS };

int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXOUT];
char out[MAXOUT];

int prevtoken = 0;
int errorflag = 0;

void dcl(void);
void dirdcl(void);
int gettoken(void);
int getch(void);
void ungetch(int);
void errmsg(const char *msg);
void recoverline(void);
int istypespec(const char *s);
int istypequal(const char *s);

void dcl(void)
{
    int ns;

    for (ns = 0; gettoken() == '*'; )
        ns++;

    dirdcl();

    while (ns-- > 0)
        strcat(out, " pointer to");
}

void dirdcl(void)
{
    int type;

    if (tokentype == '(') {
        dcl();
        if (tokentype != ')')
            errmsg("missing )");
    } else if (tokentype == NAME) {
        strcpy(name, token);
    } else {
        errmsg("expected name or (dcl)");
        return;
    }

    while ((type = gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS) {
            if (strcmp(token, "()") == 0) {
                strcat(out, " function returning");
            } else {
                strcat(out, " function expecting ");
                strcat(out, token);
                strcat(out, " returning");
            }
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
}

int gettoken(void)
{
    int c, getch(void);
    void ungetch(int);
    char *p = token;

    if (prevtoken) {
        prevtoken = 0;
        return tokentype;
    }

    while ((c = getch()) == ' ' || c == '\t')
        ;

    if (c == '(') {
        int next = getch();

        if (next == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            *p++ = '(';
            while (next != ')' && next != EOF && p < token + MAXTOKEN - 2) {
                *p++ = next;
                next = getch();
            }
            if (next == ')') {
                *p++ = ')';
                *p = '\0';

                if (strcmp(token, "()") == 0)
                    return tokentype = PARENS;

                return tokentype = PARENS;
            } else {
                *p = '\0';
                errmsg("missing ) in parameter list");
                return tokentype = PARENS;
            }
        }
    } else if (c == '[') {
        *p++ = c;
        while ((*p++ = getch()) != ']' && *(p - 1) != EOF)
            ;
        *(p) = '\0';
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

void errmsg(const char *msg)
{
    printf("error: %s\n", msg);
    errorflag = 1;
}

void recoverline(void)
{
    while (tokentype != '\n' && tokentype != EOF)
        tokentype = gettoken();
}

int istypespec(const char *s)
{
    return strcmp(s, "char") == 0 ||
           strcmp(s, "int") == 0 ||
           strcmp(s, "void") == 0 ||
           strcmp(s, "double") == 0 ||
           strcmp(s, "float") == 0 ||
           strcmp(s, "short") == 0 ||
           strcmp(s, "long") == 0 ||
           strcmp(s, "signed") == 0 ||
           strcmp(s, "unsigned") == 0;
}

int istypequal(const char *s)
{
    return strcmp(s, "const") == 0 ||
           strcmp(s, "volatile") == 0;
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

    while ((type = gettoken()) != EOF) {
        errorflag = 0;
        out[0] = '\0';
        name[0] = '\0';
        datatype[0] = '\0';

        if (type == '\n')
            continue;

        if (type != NAME) {
            errmsg("datatype expected");
            recoverline();
            continue;
        }

        strcpy(datatype, token);

        while ((type = gettoken()) == NAME &&
               (istypespec(token) || istypequal(token))) {
            strcat(datatype, " ");
            strcat(datatype, token);
        }

        prevtoken = 1;
        dcl();

        if (!errorflag && tokentype != '\n' && tokentype != EOF)
            errmsg("syntax error");

        if (errorflag) {
            recoverline();
        } else {
            printf("%s: %s %s\n", name, out, datatype);
        }
    }

    return 0;
}
