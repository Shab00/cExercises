#include "addressbook.h"
#include <cjson/cJSON.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        printf("%d: %s | %s | %s | %s\n", i, book->list[i].name, book->list[i].phone, book->list[i].email, book->list[i].address);
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

    if (fwrite("ADDR", 1, 4, f) != 4) {
        fclose(f);
        return -1;
    }

    uint8_t version = 0x02;
    if (fwrite(&version, 1, 1, f) != 1) {
        fclose(f);
        return -1;
    }

    uint8_t rec_size = (uint8_t)sizeof(Contact);
    if (fwrite(&rec_size, 1, 1, f) != 1) {
        fclose(f);
        return -1;
    }

    uint32_t count32 = (uint32_t)book->count;
    if (fwrite(&count32, 4, 1, f) != 1) {
        fclose(f);
        return -1;
    }

    uint8_t reserved[6] = {0};
    if (fwrite(reserved, 1, 6, f) != 6) {
        fclose(f);
        return -1;
    }

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
        if (fwrite(book->list[i].address, sizeof(book->list[i].address), 1, f) != 1) {
            fclose(f);
            return -1;
        }
    }

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
        fprintf(stderr, "Note: %s not found; starting empty.\n", filename);
        return 0;
    }

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
    if (version != 0x02) {
        fprintf(stderr, "Error: unsupported file version %u (expected 2).\n", version);
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
        if (fread(book->list[i].address, sizeof(book->list[i].address), 1, f) != 1) {
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

static void json_write_escaped(FILE *f, const char *s) {
    for (const char *p = s; *p != '\0'; p++) {
        switch (*p) {
            case '"':  fputs("\\\"", f); break;
            case '\\': fputs("\\\\", f); break;
            case '\n': fputs("\\n", f);  break;
            case '\r': fputs("\\r", f);  break;
            case '\t': fputs("\\t", f);  break;
            default:   fputc(*p, f);     break;
        }
    }
}

int storage_save_to_json(const AddressBook *book, const char *filename) {
    if (book == NULL || filename == NULL) {
        return -1;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return -1;
    }

    if (fprintf(f, "{\n  \"contacts\": [\n") < 0) {
        fclose(f);
        return -1;
    }

    for (int i = 0; i < book->count; i++) {
        if (fprintf(f, "    {\n") < 0) {
            fclose(f);
            return -1;
        }

        if (fprintf(f, "      \"name\": \"") < 0) { fclose(f); return -1; }
        json_write_escaped(f, book->list[i].name);
        if (fprintf(f, "\",\n") < 0) { fclose(f); return -1; }

        if (fprintf(f, "      \"phone\": \"") < 0) { fclose(f); return -1; }
        json_write_escaped(f, book->list[i].phone);
        if (fprintf(f, "\",\n") < 0) { fclose(f); return -1; }

        if (fprintf(f, "      \"email\": \"") < 0) { fclose(f); return -1; }
        json_write_escaped(f, book->list[i].email);
        if (fprintf(f, "\",\n") < 0) { fclose(f); return -1; }

        if (fprintf(f, "      \"address\": \"") < 0) { fclose(f); return -1; }
        json_write_escaped(f, book->list[i].address);
        if (fprintf(f, "\"\n") < 0) { fclose(f); return -1; }

        if (fprintf(f, "    }") < 0) { fclose(f); return -1; }

        if (i < book->count - 1) {
            if (fprintf(f, ",") < 0) { fclose(f); return -1; }
        }
        if (fprintf(f, "\n") < 0) { fclose(f); return -1; }
    }

    if (fprintf(f, "  ]\n}\n") < 0) {
        fclose(f);
        return -1;
    }

    if (fclose(f) != 0) {
        return -1;
    }
    return 0;
}

int storage_load_from_json(AddressBook *book, const char *filename) {
    if (book == NULL || filename == NULL) {
        return -1;
    }

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Note: %s not found; starting empty.\n", filename);
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size < 0 || size > 1024 * 1024) {
        fprintf(stderr, "Error: file too large or seek failed.\n");
        fclose(f);
        return -1;
    }

    char *buf = malloc((size_t)size + 1);
    if (buf == NULL) {
        fclose(f);
        return -1;
    }

    if (fread(buf, 1, (size_t)size, f) != (size_t)size) {
        fprintf(stderr, "Error: could not read file.\n");
        free(buf);
        fclose(f);
        return -1;
    }
    buf[size] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(buf);
    free(buf);

    if (root == NULL) {
        fprintf(stderr, "Error: invalid JSON in %s.\n", filename);
        return -1;
    }

    cJSON *contacts = cJSON_GetObjectItem(root, "contacts");
    if (!cJSON_IsArray(contacts)) {
        fprintf(stderr, "Error: missing 'contacts' array.\n");
        cJSON_Delete(root);
        return -1;
    }

    int count = 0;
    cJSON *item;
    cJSON_ArrayForEach(item, contacts) {
        if (count >= 100) {
            fprintf(stderr, "Error: too many contacts in file.\n");
            cJSON_Delete(root);
            return -1;
        }

        cJSON *name    = cJSON_GetObjectItem(item, "name");
        cJSON *phone   = cJSON_GetObjectItem(item, "phone");
        cJSON *email   = cJSON_GetObjectItem(item, "email");
        cJSON *address = cJSON_GetObjectItem(item, "address");

        if (!cJSON_IsString(name) || !cJSON_IsString(phone) || !cJSON_IsString(email)) {
            fprintf(stderr, "Error: contact %d missing required field.\n", count);
            cJSON_Delete(root);
            return -1;
        }

        snprintf(book->list[count].name,  sizeof(book->list[count].name),
                 "%s", name->valuestring);
        snprintf(book->list[count].phone, sizeof(book->list[count].phone),
                 "%s", phone->valuestring);
        snprintf(book->list[count].email, sizeof(book->list[count].email),
                 "%s", email->valuestring);

        if (cJSON_IsString(address)) {
            snprintf(book->list[count].address, sizeof(book->list[count].address),
                     "%s", address->valuestring);
        } else {
            book->list[count].address[0] = '\0';
        }

        count++;
    }

    cJSON_Delete(root);
    book->count = count;
    return 0;
}
