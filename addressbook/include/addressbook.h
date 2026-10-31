#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

typedef struct {
    char name[50];
    char phone[15];
    char email[50];
    char address[100];
} Contact;

typedef struct {
    Contact list[100];
    int count;
} AddressBook;

void book_init(AddressBook *book); 

int contact_add(AddressBook *book, const Contact *c);

int contact_search_by_name(const AddressBook *book, const char *name);

int contact_delete(AddressBook *book, int index);

void contact_list_all(const AddressBook *book);

int storage_save_to_file(const AddressBook *book, const char *filename);

int storage_load_from_file(AddressBook *book, const char *filename);

int storage_save_to_json(const AddressBook *book, const char *filename);

int storage_load_from_json(AddressBook *book, const char *filename);

#endif // ADDRESSBOOK_H
