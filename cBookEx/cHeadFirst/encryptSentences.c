#include <stdio.h>
#include <string.h>
#include "encrypt.h"

int main() 
{
    const char *input_sentences[] = {
        "This is a sentence",
        "this also is a sentnce",
        "also sentence",
        "your mum"
    };

    int lenOfList = sizeof(input_sentences) / sizeof(input_sentences[0]);
    char sentences[4][100]; 

    for (int i = 0; i < lenOfList; i++) {
        strncpy(sentences[i], input_sentences[i], 99);
        sentences[i][99] = '\0'; 
        encrypt(sentences[i]);
        printf("this is encrypted: %s\n", sentences[i]);
    }
    return 0;
}
