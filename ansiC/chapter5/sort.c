#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINES 5000
#define MAXLEN 1000
#define MAXSTR 1000

#define NUMERIC 1
#define DECR    2
#define FOLD    4
#define DIR     8

char *lineptr[MAXLINES];
int option = 0;
int pos1 = 0;
int pos2 = 0;

int readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int nlines, int decr);
void my_qsort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *, const char *);
int charcmp(const char *, const char *);
void swap(void *v[], int i, int j);
int getline_custom(char *s, int lim);
void substr(const char *s, char *str);
void readargs(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int nlines;
    int (*comp)(const char *, const char *);

    readargs(argc, argv);

    if (option & NUMERIC)
        comp = numcmp;
    else
        comp = charcmp;

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        my_qsort((void **) lineptr, 0, nlines - 1,
                 (int (*)(void *, void *)) comp);
        writelines(lineptr, nlines, option & DECR);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}

void readargs(int argc, char *argv[])
{
    int c;

    while (--argc > 0) {
        if ((*++argv)[0] == '-' && !isdigit((unsigned char)argv[0][1])) {
            while ((c = *++argv[0])) {
                switch (c) {
                    case 'd':
                        option |= DIR;
                        break;
                    case 'f':
                        option |= FOLD;
                        break;
                    case 'n':
                        option |= NUMERIC;
                        break;
                    case 'r':
                        option |= DECR;
                        break;
                    default:
                        printf("sort: illegal option %c\n", c);
                        printf("Usage: sort -dfnr [+pos1] [-pos2]\n");
                        exit(1);
                }
            }
        } else if (argv[0][0] == '+') {
            pos1 = atoi(argv[0] + 1);
            if (pos1 < 0) {
                printf("Usage: sort -dfnr [+pos1] [-pos2]\n");
                exit(1);
            }
        } else if (argv[0][0] == '-') {
            pos2 = atoi(argv[0] + 1);
            if (pos2 < 0) {
                printf("Usage: sort -dfnr [+pos1] [-pos2]\n");
                exit(1);
            }
        } else {
            printf("Usage: sort -dfnr [+pos1] [-pos2]\n");
            exit(1);
        }
    }

    if (pos1 > pos2 && pos2 > 0) {
        printf("sort: invalid field positions\n");
        exit(1);
    }
}

void my_qsort(void *v[], int left, int right, int (*comp)(void *, void *))
{
    int i, last;

    if (left >= right)
        return;

    swap(v, left, (left + right) / 2);
    last = left;

    for (i = left + 1; i <= right; i++)
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);

    swap(v, left, last);
    my_qsort(v, left, last - 1, comp);
    my_qsort(v, last + 1, right, comp);
}

int numcmp(const char *s1, const char *s2)
{
    double v1, v2;
    char str[MAXSTR];

    substr(s1, str);
    v1 = atof(str);
    substr(s2, str);
    v2 = atof(str);

    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

int charcmp(const char *s, const char *t)
{
    char a, b;
    int i, j, endpos;
    int fold = (option & FOLD) ? 1 : 0;
    int dir = (option & DIR) ? 1 : 0;

    i = j = pos1;

    if (pos2 > 0)
        endpos = pos2;
    else if ((int)strlen(s) > (int)strlen(t))
        endpos = strlen(s);
    else
        endpos = strlen(t);

    do {
        if (dir) {
            while (i < endpos &&
                   s[i] != '\0' &&
                   !isalnum((unsigned char)s[i]) &&
                   s[i] != ' ')
                i++;

            while (j < endpos &&
                   t[j] != '\0' &&
                   !isalnum((unsigned char)t[j]) &&
                   t[j] != ' ')
                j++;
        }

        a = (i < endpos && s[i] != '\0')
                ? (fold ? tolower((unsigned char)s[i]) : s[i])
                : '\0';
        b = (j < endpos && t[j] != '\0')
                ? (fold ? tolower((unsigned char)t[j]) : t[j])
                : '\0';

        if (a == b && a == '\0')
            return 0;

        if (i < endpos && s[i] != '\0')
            i++;
        if (j < endpos && t[j] != '\0')
            j++;
    } while (a == b);

    return a - b;
}

void substr(const char *s, char *str)
{
    int i, j, len;

    len = strlen(s);
    if (pos2 > 0 && len > pos2)
        len = pos2;

    if (pos1 > len)
        pos1 = len;

    for (j = 0, i = pos1; i < len; i++, j++)
        str[j] = s[i];

    str[j] = '\0';
}

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int getline_custom(char *s, int lim)
{
    int c;
    char *start = s;

    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        *s++ = c;
    if (c == '\n')
        *s++ = c;
    *s = '\0';
    return s - start;
}

int readlines(char *lineptr[], int maxlines)
{
    int len, nlines = 0;
    char *p, line[MAXLEN];

    while ((len = getline_custom(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || (p = malloc(len + 1)) == NULL)
            return -1;
        strcpy(p, line);
        lineptr[nlines++] = p;
    }
    return nlines;
}

void writelines(char *lineptr[], int nlines, int decr)
{
    int i;

    if (decr) {
        for (i = nlines - 1; i >= 0; i--)
            printf("%s", lineptr[i]);
    } else {
        for (i = 0; i < nlines; i++)
            printf("%s", lineptr[i]);
    }
}
