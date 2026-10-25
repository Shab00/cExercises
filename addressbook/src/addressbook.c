#include "addressbook.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
    if (book == NULL) {
        return -1;
    }

    if (index < 0 || index >= book->count) {
        return -1;
    }

    for (int i = index; i < book->count - 1; i++) {
        book->list[i] = book->list[i + 1];
    }

    book->count--;

    return 0;

}

void contact_list_all(const AddressBook *book) {
    if (book == NULL) {
        return;
    }

    if (book->count <= 0) {
        printf("Empty Address Book!\n");
        return;
    }

    for (int i = 0; i < book->count; i++) {
        printf("%d: %s | %s | %s\n", i, book->list[i].name, book->list[i].phone, book->list[i].email);
    }
}

int storage_save_to_file(const AddressBook *book, const char *filename) {
    if (book == NULL || filename == NULL) {
        return -1;
    }

    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return -1;
    }

    // --- Header: 16 bytes total ---

    // Magic: "ADDR"
    if (fwrite("ADDR", 1, 4, f) != 4) {
        fclose(f);
        return -1;
    }

    // Version: 0x01
    uint8_t version = 0x01;
    if (fwrite(&version, 1, 1, f) != 1) {
        fclose(f);
        return -1;
    }

    // Record size: sizeof(Contact)
    uint8_t rec_size = (uint8_t)sizeof(Contact);
    if (fwrite(&rec_size, 1, 1, f) != 1) {
        fclose(f);
        return -1;
    }

    // Count: 4 bytes, uint32 little-endian (native)
    uint32_t count32 = (uint32_t)book->count;
    if (fwrite(&count32, 4, 1, f) != 1) {
        fclose(f);
        return -1;
    }

    // Reserved: 6 zero bytes
    uint8_t reserved[6] = {0};
    if (fwrite(reserved, 1, 6, f) != 6) {
        fclose(f);
        return -1;
    }

    // --- Records: 115 bytes each (name 50 + phone 15 + email 50) ---

    for (int i = 0; i < book->count; i++) {
        if (fwrite(book->list[i].name, sizeof(book->list[i].name), 1, f) != 1) {
            fclose(f);
            return -1;
        }
        if (fwrite(book->list[i].phone, sizeof(book->list[i].phone), 1, f) != 1) {
            fclose(f);
            return -1;
        }
        if (fwrite(book->list[i].email, sizeof(book->list[i].email), 1, f) != 1) {
            fclose(f);
            return -1;
        }
    }

    // --- Flush and close ---

    if (fclose(f) != 0) {
        return -1;
    }

    return 0;
}

int storage_load_from_file(AddressBook *book, const char *filename) {
    if (book == NULL || filename == NULL) {
        return -1;
    }

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        // Missing file is not an error — first run, empty book.
        fprintf(stderr, "Note: %s not found; starting empty.\n", filename);
        return 0;
    }

    // --- Header: 16 bytes total, read and validate ---

    char magic[4];
    if (fread(magic, 1, 4, f) != 4) {
        fprintf(stderr, "Error: %s is too short to be an address book.\n", filename);
        fclose(f);
        return -1;
    }
    if (memcmp(magic, "ADDR", 4) != 0) {
        fprintf(stderr, "Error: %s is not an address book file.\n", filename);
        fclose(f);
        return -1;
    }

    uint8_t version;
    if (fread(&version, 1, 1, f) != 1) {
        fclose(f);
        return -1;
    }
    if (version != 0x01) {
        fprintf(stderr, "Error: unsupported file version %u.\n", version);
        fclose(f);
        return -1;
    }

    uint8_t rec_size;
    if (fread(&rec_size, 1, 1, f) != 1) {
        fclose(f);
        return -1;
    }
    if (rec_size != (uint8_t)sizeof(Contact)) {
        fprintf(stderr, "Error: record size mismatch (file=%u, expected=%zu).\n",
                rec_size, sizeof(Contact));
        fclose(f);
        return -1;
    }

    uint32_t count32;
    if (fread(&count32, 4, 1, f) != 1) {
        fclose(f);
        return -1;
    }
    if (count32 > 100) {
        fprintf(stderr, "Error: file claims %u records, exceeds capacity.\n", count32);
        fclose(f);
        return -1;
    }

    uint8_t reserved[6];
    if (fread(reserved, 1, 6, f) != 6) {
        fclose(f);
        return -1;
    }

    // --- Records ---

    int count = (int)count32;
    for (int i = 0; i < count; i++) {
        if (fread(book->list[i].name, sizeof(book->list[i].name), 1, f) != 1) {
            fprintf(stderr, "Error: file truncated at record %d.\n", i);
            fclose(f);
            return -1;
        }
        if (fread(book->list[i].phone, sizeof(book->list[i].phone), 1, f) != 1) {
            fprintf(stderr, "Error: file truncated at record %d.\n", i);
            fclose(f);
            return -1;
        }
        if (fread(book->list[i].email, sizeof(book->list[i].email), 1, f) != 1) {
            fprintf(stderr, "Error: file truncated at record %d.\n", i);
            fclose(f);
            return -1;
        }
    }

    book->count = count;

    if (fclose(f) != 0) {
        return -1;
    }
    return 0;
}
