#include <stdio.h>

int in_string(int delim);
int single_quote(int squote);
int comment(int back);
int paragraph(int star);

int main()
{
    int c;

    c = 0;
    while((c = getchar()) != EOF) {
        if (c == '"') {
            putchar(c);
            in_string(c); 
        } else if (c == '\'') {
            putchar(c);
            single_quote(c);
        } else if (c == '/') {
            int next = getchar();
            if (next == EOF) {
                return 0 ;
            } else if (next == '/') {
                comment(c);
            } else if (next == '*') {
                paragraph(c);
            }
        } else {
            putchar(c);
        }
    }

    return 0;
}

int in_string(int delim)
{

    int j;
    while ((j = getchar()) != EOF) {
        if (j == '\\') {
            putchar('\\');
            int k = getchar();
            if (k == EOF) return 0;
            putchar(k);
            continue;
        }
        if (j == delim) {
            putchar(delim);
            return 1;
        }
        putchar(j);
    }
    return 0;
}

int single_quote(int squote)
{
    int j;
    while ((j = getchar()) != EOF) {
        if (j == '\\') {
            putchar('\\');
            int k = getchar();
            if (k == EOF) return 0;
            putchar(k);
            continue;
        } 
        if (j == squote) {
            putchar(squote);
            return 1;
        }
        putchar(j);
    }
    return 0;
}

int comment(int back)
{
    int j;
    while ((j = getchar()) != EOF) {
        if (j == '\\') {
            putchar('\\');
            int k = getchar();
            if (k == EOF) return 0;
            putchar(k);
            continue;
        } 
        if (j == '\n') {
            putchar('\n');
            return 1;
        }
    }
    return 0;

}

int paragraph(int star)
{
    int j;
    while ((j = getchar()) != EOF) {
        if (j == '\\') {
            putchar('\\');
            int k = getchar();
            if (k == EOF) return 0;
            putchar(k);
            continue;
        } 
        if (j == '*') {
            int next = getchar();
            if (next == EOF) {
                return 0;
            } else if (next == '/') {
                putchar('\n');
                return 1;
            }
        }
    }
    return 0;
}
