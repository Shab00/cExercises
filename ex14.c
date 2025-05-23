#include <stdio.h>
#include <ctype.h>
#include <string.h>
// forward decleration
void print_letters(char arg[], int length);

void print_arguments(int argc, char *argv[])
{

    for (int i = 0; i < argc; i++) {
        int length = strlen(argv[i]);
        printf("Arguments %d ('%s') has length: %d\n", i, argv[i], length);
        print_letters(argv[i], length);
    }
}

void print_letters(char arg[], int length)
{
    int i = 0;

    for (i = 0; i < length; i++) {
        char ch = arg[i];

        if (isalpha(ch) || isblank(ch)) {
            printf("'%c' == %d ", ch, ch);
        }
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    print_arguments(argc, argv);
    return 0;
}
