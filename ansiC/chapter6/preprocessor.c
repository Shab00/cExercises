#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define HASHSIZE 101

struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) {
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else {
        free((void *) np->defn);
    }
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

void undef(char *s)
{
    int h;
    struct nlist *prev, *np;

    prev = NULL;
    h = hash(s);
    for (np = hashtab[h]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0)
            break;
        prev = np;
    }
    if (np != NULL) {
        if (prev == NULL)
            hashtab[h] = np->next;
        else
            prev->next = np->next;
        free((void *) np->name);
        free((void *) np->defn);
        free((void *) np);
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
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

void ungets(char s[])
{
    int len = strlen(s);
    while (len > 0)
        ungetch(s[--len]);
}

int getword(char *word, int lim)
{
    int c, getch(void);
    void ungetch(int);
    char *w = word;

    while (isspace(c = getch()))
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c) && c != '#' && c != '_') {
        *w = '\0';
        return c;
    }
    for ( ; --lim > 0; w++)
        if (!isalnum(*w = getch()) && *w != '_') {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

void error(int, char *);
void getdef(void);
void skipblanks(void);


int main()
{
    char w[MAXWORD];
    struct nlist *p;

    while (getword(w, MAXWORD) != EOF)
        if (strcmp(w, "#") == 0) 
            getdef();
        else if (!isalpha(w[0]) && w[0] != '_')
            printf("%s", w); 
        else if ((p = lookup(w)) == NULL)
            printf("%s", w);
        else
            ungets(p->defn);
    return 0;
}

void getdef(void)
{
    int c, i;
    char def[MAXWORD], dir[MAXWORD], name[MAXWORD];

    skipblanks();
    if (!isalpha(getword(dir, MAXWORD)))
        error(dir[0], "getdef: expecting a directive after #");
    else if (strcmp(dir, "define") == 0) {
        skipblanks();
        if (!isalpha(getword(name, MAXWORD)) && name[0] != '_')
            error(name[0], "getdef: non-alpha - name expected");
        else {
            skipblanks();
            for (i = 0; i < MAXWORD - 1; i++)
                if ((def[i] = getch()) == EOF || def[i] == '\n')
                    break;
            def[i] = '\0';
            if (i <= 0)
                error('\n', "getdef: incomplete define");
            else 
                install(name, def);
        }
    } else if (strcmp(dir, "undef") == 0) {
        skipblanks();
        if (!isalpha(getword(name, MAXWORD)))
            error(name[0], "getdef: non-alpha in undef");
        else
            undef(name);
    } else
        error(dir[0], "getdef: expecting a directive after #");
}

void error(int c, char *s)
{
    printf("error: %s\n", s);
    while (c != EOF && c != '\n')
        c = getch();
}

void skipblanks(void)
{
    int c;
    while ((c = getch()) == ' ' || c == '\t')
        ;
    ungetch(c);
}
