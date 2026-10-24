#include "../include/addressbook.h"

#include <stdio.h>
#include <string.h>

void book_init(AddressBook *book) {
    if (book == NULL) {
        return;
    }

    book->count = 0;

} 

int contact_add(AddressBook *book, const Contact *c) {
    if (book == NULL || c == NULL || book->count > 99) {
        return -1;
    }

    book->list[book->count] = *c;

    book->count++;

    return 0;

}

int contact_search_by_name(const AddressBook *book, const char *name) {
    if (book == NULL || name == NULL) {
        return -1;
    }

    for (int i = 0; i < book->count; i++) {
        if (strcmp(book->list[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

int contact_delete(AddressBook *book, int index) {

}
