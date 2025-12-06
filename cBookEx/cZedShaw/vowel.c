#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc !=2){
        printf("ERROR: You need one argument.\n");
        // this is how to abort a program
        return 1;
    }
    
    for (int i = 0, letter = argv[1][i]; argv[1][i] != '\0'; i++, letter = argv[1][i]){
        
        char lowercase = letter | 32;
        char vowels[] = {'a', 'e', 'i', 'o', 'u'};
        int is_vowel = 0;
        for(int j =0; j < 5; j++){ 
            if (lowercase == vowels[j]){
                printf("%d: '%c'\n", i, vowels[j]);
                is_vowel = 1;
                break;
            }
        }

        if (!is_vowel && argv[1][i+1] == '\0'){
            printf("%d: 'Y'\n", i);
        }

    }            
    return 0;
}
