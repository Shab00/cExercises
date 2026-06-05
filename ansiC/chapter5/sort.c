#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINES 5000
#define MAXLEN 1000
#define NUMERIC 1 
#define DECR 2

char *lineptr[MAXLINES];

int readlines(char *lineptr[], int maxlines);
void writelines(char *lineptr[], int nlines, int decr);

void my_qsort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int numcmp(const char *, const char *);
void swap(void *v[], int i, int j);

int main(int argc, char *argv[])
{
    int nlines;
    int option = 0;

    while (--argc > 0 && (*++argv)[0] == '-') {
        int c;
        char *arg = *argv;
        while ((c = *++arg)) {
            switch (c) {
                case 'n':
                    option |= NUMERIC; // numeric sort
                    break;
                case 'r':
                    option |= DECR;    // reverse order
                    break;
                default:
                    printf("sort: illegal option %c\n", c);
                    printf("Usage: sort -n -r\n");
                    exit(1);
            }
        }
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        my_qsort((void **) lineptr, 0, nlines - 1,
            (int (*)(void *, void *))(option & NUMERIC ? numcmp : strcmp));
        writelines(lineptr, nlines, option & DECR);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
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
    double v1 = atof(s1);
    double v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

void swap(void *v[], int i, int j)
{
    void *temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int getline_custom(char *s, int lim) {
    int c;
    char *start = s;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        *s++ = c;
    if (c == '\n')
        *s++ = c;
    *s = '\0';
    return s - start;
}

int readlines(char *lineptr[], int maxlines) {
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

void writelines(char *lineptr[], int nlines, int decr) {
    int i;
    if (decr) {
        for (i = nlines - 1; i >= 0; i--)
            printf("%s", lineptr[i]);
    } else {
        for (i = 0; i < nlines; i++)
            printf("%s", lineptr[i]);
    }
}
