#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
    int id;
    int set;
    char *name;  // Dynamically allocated based on max_data
    char *email; // Dynamically allocated based on max_data
};

struct Database {
    int max_data;
    int max_rows;
    struct Address *rows;  // Dynamically allocated based on max_rows
};

struct Connection {
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if (conn) {
        Database_close(conn);
    }

    if (errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        die("Failed to load database.", conn);

    // Allocate memory for rows based on max_rows
    conn->db->rows = malloc(sizeof(struct Address) * conn->db->max_rows);
    if (!conn->db->rows)
        die("Memory error", conn);

    rc = fread(conn->db->rows, sizeof(struct Address), conn->db->max_rows, conn->file);
    if (rc != conn->db->max_rows)
        die("Failed to load rows.", conn);
}

struct Connection *Database_open(const char *filename, char mode, int max_data, int max_rows)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        die("Memory error", NULL);

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die("Memory error", conn);

    conn->db->max_data = max_data;
    conn->db->max_rows = max_rows;

    // Allocate memory for rows based on max_rows
    conn->db->rows = malloc(sizeof(struct Address) * max_rows);
    if (!conn->db->rows)
        die("Memory error", conn);

    if (mode == 'c') {
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    }

    if (!conn->file)
        die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if (conn) {
        if (conn->file) {
            fclose(conn->file);
            conn->file = NULL;
        }

        if (conn->db) {
            if (conn->db->rows) {
                // Free dynamically allocated memory for rows and their data
                for (int i = 0; i < conn->db->max_rows; i++) {
                    free(conn->db->rows[i].name);
                    free(conn->db->rows[i].email);
                }
                free(conn->db->rows);
            }
            free(conn->db);
        }

        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        die("Failed to write database.", conn);

    rc = fwrite(conn->db->rows, sizeof(struct Address), conn->db->max_rows, conn->file);
    if (rc != conn->db->max_rows)
        die("Failed to write rows.", conn);

    rc = fflush(conn->file);
    if (rc == -1)
        die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    for (int i = 0; i < conn->db->max_rows; i++) {
        struct Address addr = {.id = i, .set = 0};

        // Allocate memory for name and email
        addr.name = malloc(conn->db->max_data);
        addr.email = malloc(conn->db->max_data);

        if (!addr.name || !addr.email)
            die("Memory allocation failed", conn);

        // Initialize rows
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set)
        die("Already set, delete it first", conn);

    addr->set = 1;

    // Allocate memory for name and email if not already done
    addr->name = realloc(addr->name, conn->db->max_data);
    addr->email = realloc(addr->email, conn->db->max_data);

    // Copy name and email
    strncpy(addr->name, name, conn->db->max_data - 1);
    addr->name[conn->db->max_data - 1] = '\0';

    strncpy(addr->email, email, conn->db->max_data - 1);
    addr->email[conn->db->max_data - 1] = '\0';
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if (addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};

    free(conn->db->rows[id].name);
    free(conn->db->rows[id].email);

    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    for (int i = 0; i < conn->db->max_rows; i++) {
        struct Address *cur = &conn->db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 5)
        die("USAGE: ex17 <dbfile> <action> <max_data> <max_rows>", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    int max_data = atoi(argv[3]);
    int max_rows = atoi(argv[4]);

    struct Connection *conn = Database_open(filename, action, max_data, max_rows);
    int id = 0;

    if (argc > 5)
        id = atoi(argv[5]);
    if (id >= max_rows)
        die("There's not that many records.", conn);

    switch (action) {
    case 'c':
        Database_create(conn);
        Database_write(conn);
        break;

    case 'g':
        if (argc != 5)
            die("Need an id to get", conn);

        Database_get(conn, id);
        break;

    case 's':
        if (argc != 7)
            die("Need id, name, email to set", conn);

        Database_set(conn, id, argv[5], argv[6]);
        Database_write(conn);
        break;

    case 'd':
        if (argc != 5)
            die("Need id to delete", conn);

        Database_delete(conn, id);
        Database_write(conn);
        break;

    case 'l':
        Database_list(conn);
        break;

    default:
        die("Invalid action: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}
