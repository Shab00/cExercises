#include <stdio.h>
#include <string.h>

#define MAXLINES 5000
#define MAXLEN 1000
#define MAXSTOR 1000
#define ALLOCSIZE 100000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;
char *lineptr[MAXLINES];
char linestor[MAXSTOR];
char *alloc(int);

char *alloc(int n);
int my_getline(char *, int);
int readlines(char *lineptr[], char linestor[], int maxlines, int maxstor);
void writelines(char *lineptr[], int nlines);
void my_qsort(char *lineptr[], int left, int right);
void swap(char *v[], int i, int j);

int main()
{
    int nlines;

    if ((nlines = readlines(lineptr, linestor, MAXLINES, MAXSTOR)) >= 0) {
        my_qsort(lineptr, 0, nlines - 1);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("error: input too big to sort\n");
        return 1;
    }
}

int readlines(char *lineptr[], char linestor[], int maxlines, int maxstor)
{
    int len, nlines = 0;
    char line[MAXLEN];
    char *p = linestor;
    char *linestop = linestor + maxstor;

    while ((len = my_getline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || p + len > linestop)
            return -1;
        line[len-1] = '\0';
        strcpy(p, line);
        lineptr[nlines++] = p;
        p += len;
    }
    return nlines;
}

void writelines(char *lineptr[], int nlines) 
{
    while (nlines-- > 0) {
        printf("%s\n", *lineptr++);
    }
}

void my_qsort(char *v[], int left, int right)
{
    int i, last;

    if (left >= right) {
        return;
    }
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (strcmp(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    my_qsort(v, left, last - 1);
    my_qsort(v, last + 1, right);
}

void swap(char *v[], int i, int j)
{
    char *temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

char *alloc(int n)
{
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else {
        return NULL;
    }
}

int my_getline(char *s, int lim)
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
