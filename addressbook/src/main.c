#include "addressbook.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main() 
{
    AddressBook book;
    book_init(&book);

    Contact personOne;
    Contact personTwo;
    Contact personThree;

    strcpy(personOne.name, "Alice");
    strcpy(personOne.phone, "07940161559");
    strcpy(personOne.email, "Al@a.com");
    
    strcpy(personTwo.name, "Tom");
    strcpy(personTwo.phone, "07770325022");
    strcpy(personTwo.email, "To@t.com");
    
    strcpy(personThree.name, "Dick");
    strcpy(personThree.phone, "07770415033");
    strcpy(personThree.email, "Di@d.com");
    
    contact_add(&book, &personOne);
    contact_add(&book, &personTwo);
    contact_add(&book, &personThree);
    
    const char pone[] = "Alice";
    int found = contact_search_by_name(&book, pone);
    printf("Alice found at index: %d\n", found);

    int indexTwo = 1;
    contact_delete(&book, indexTwo);

    contact_list_all(&book);

    storage_save_to_file(&book, "test.db");
    book_init(&book);
    storage_load_from_file(&book, "test.db");
    contact_list_all(&book);

    return 0;
}
