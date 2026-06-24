#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE 100

char *getWord(char *word, int lim);
int comment(void);
int getch(void);
void ungetch(int);

char buf[BUFSIZE];
int bufp = 0;

int main() 
{
    char *s = "Hi Bashaar. _my_var , #define MAX 10 /* comment */ \"hello \\\" world\" ";
    size_t n = 0;
    
    for (int i = strlen(s) - 1; i >= 0; i--) {
        ungetch(s[i]);
    }

    char current_word[BUFSIZE]; 
    char *p;

    while ((p = getWord(current_word, BUFSIZE)) != NULL) {
        printf("%zu: %s\n", n, p);
        ++n;
    }

    printf("\nThere are %zu words in the string\n", n);

    return 0;
}

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : EOF;
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE) {
        printf("ungetch: too many characters\n");
    } else {
        buf[bufp++] = c;
    }
}

char *getWord(char *word, int lim) 
{
    int c, d;
    char *w = word;

    while (isspace(c = getch())) {
        ;
    }

    if (c == EOF || c == '\0') {
        return NULL;
    }

    *w++ = c;

    if (isalpha(c) || c == '_' || c == '#') {
        for (; --lim > 1; w++) {
            if (!isalnum(*w = getch()) && *w != '_') {
                ungetch(*w);
                break;
            }
        }
        *w = '\0';
        return word;
    }

    if (c == '\'' || c == '"') {
        for (; --lim > 1; w++) {
            if ((*w = getch()) == '\\') {
                if (--lim > 1) {
                    *++w = getch();
                }
            } else if (*w == c) {
                w++;
                break;
            } else if (*w == EOF) {
                break;
            }
        }
        *w = '\0';
        return word;
    }

    if (c == '/') {
        if ((d = getch()) == '*') {
            comment();
            return getWord(word, lim); 
        } else {
            ungetch(d);
        }
    }

    *w = '\0';
    return word;
}

int comment(void)
{
    int c;
    while ((c = getch()) != EOF) {
        if (c == '*') {
            if ((c = getch()) == '/') {
                break;
            } else {
                ungetch(c);
            }
        }
    }
    return c;
}
