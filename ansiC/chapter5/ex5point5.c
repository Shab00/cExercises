#include <stdio.h>
#include <string.h>

int my_strncmp(char s[], char t[], int n); 
void my_strncpy(char s[], char t[], int n);
void my_strncat(char s[], char t[], int n);
void print_ascii_codes(const char s[]);

int main()
{
    char s1[100] = "Hello";
    char t1[100] = "World";

    char s2[100];
    char t2[100] = "Hi";
    int n = 3;

    printf("Testing my_strncmp:\n");
    printf("Comparing \"%s\" and \"%s\" for %d chars: %d\n",
            s1, t1, n, my_strncmp(s1, t1, n));

    printf("\nTesting my_strncpy:\n");
    memset(s2, 0, sizeof(s2));
    my_strncpy(s2, t2, 5);
    printf("After copying \"%s\" (n=5): \"%s\"\n", t2, s2);
    printf("ASCII codes of s2: ");
    print_ascii_codes(s2);

    printf("\nTesting my_strncat:\n");
    char s3[100] = "abc";
    char t3[] = "defgh";
    my_strncat(s3, t3, 3);
    printf("After strncat(\"abc\", \"defgh\", 3): \"%s\"\n", s3);

    printf("ASCII codes of s3: ");
    print_ascii_codes(s3);

    return 0;
}

int my_strncmp(char *s, char *t, int n) 
{
    while (n-- > 0){
        if (*s != *t){
            return *s - *t;
        }
        if (*s == '\0') {
            return 0;
        }
        s++;
        t++;
    }
    return 0;

}

void my_strncat(char *s, char *t, int n) 
{

    while (*s) {
        s++;
    }

    while (n-- && (*t)) {
        *s++ = *t++;
    }
    *s = '\0';
}

void my_strncpy(char *s, char *t, int n) 
{

    while (n-- && (*t)) {
        *s++ = *t++;
    }

    while (n-- >= 0) {
        *s++ = '\0';
    }
}

void print_ascii_codes(const char *s) 
{
    while ((*s)) {
        printf("%d ", *s);
        s++;
    }
    printf("\n");
}
