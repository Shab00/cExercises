#include <stdio.h>

int main(int argc, char *argv[])
{

    char **arg_ptr = argv + 1;

    for (int i = 0; i < argc; i++) {
        printf("Arguments %d: %s and Adress is: %p\n", i, *(arg_ptr + i),
                (void*)(arg_ptr + i));
    }
    
    // create two pointers (instead of arrays)
    int *ages = (int[]){23, 43, 12, 89, 2};  // ages is now a pointer
    char **names = (char *[]){               // names is nof pointers
        "Alan", "Frank",
        "Mary", "John", "Lisa"
    };

    // safely get the size of ages
    int count = sizeof(int) * 5 / sizeof(int); // since we losgth info
    int i = 0;

    // first way using indexing (now treated like a pointer)
    for (i = 0; i < count; i++) {
        printf("%s has %d years alive.\n", *(names + i), *(ages + i));
    }

    printf("---\n");

    // setup the arrays to the start of the arrays (treating c arrays)
    int cur_age[5];
    char *cur_names[5];

    // copying the values from the original pointers to our arrays
    for (i = 0; i < count; i++) {
        cur_age[i] = *(ages + i);
        cur_names[i] = *(names + i);
    }

    // second way using arrays (treating cur_age and cur_names like arrays)
    for (i = 0; i < count; i++) {
        printf("%s is %d years old.\n", cur_names[i], cur_age[i]);
    }

    printf("---\n");

    // third way, arrays are now treated as pointers (indexingnotation)
    for (i = 0; i < count; i++) {
        printf("%s is %d years old again.\n", *(cur_names + i), *(cur_age + i));
    }

    printf("---\n");

    // Declare pointers first, then use them in the loop
    char **cur_n = cur_names + count - 1;
    int *cur_a = cur_age + count - 1;

    // fourth way using pointer notation in a loop
    for (; cur_a >= cur_age; cur_n--, cur_a--) {
        printf("%s lived %d years so far.\n", *cur_n, *cur_a);  // Uoa
    }

    return 0;
}
