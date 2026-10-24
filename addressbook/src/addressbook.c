#include "../include/addressbook.h"

#include <stdio.h>
#include <string.h>

void book_init(AddressBook *book) {
    if (book == NULL) {
        return;
    }

    book->count = 0;

} 
