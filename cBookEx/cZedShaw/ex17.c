#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address *rows;
    int max_rows;
    int max_data;
};

struct Connection {
    FILE *file;
    struct Database *db;
};

// Function declarations
void Database_close(struct Connection *conn);
void Database_find(struct Connection *conn, const char *search_term);
void die(const char *message, struct Connection *conn);
void Address_print(struct Address *addr);
void Database_load(struct Connection *conn);
struct Connection *Database_open(const char *filename, const char *mode, int max_data, int max_rows);
void Database_write(struct Connection *conn);
void Database_create(struct Connection *conn);
void Database_set(struct Connection *conn, int id, const char *name, const char *email);
void Database_get(struct Connection *conn, int id);
void Database_delete(struct Connection *conn, int id);
void Database_list(struct Connection *conn);

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
}

struct Connection *Database_open(const char *filename, const char *mode, int max_data, int max_rows)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        die("Memory error", NULL);

    conn->db = malloc(sizeof(struct Database));
    if (!conn->db)
        die("Memory error", conn);

    conn->db->max_rows = max_rows;
    conn->db->max_data = max_data;
    conn->db->rows = malloc(sizeof(struct Address) * max_rows);

    if (!conn->db->rows)
        die("Memory error", conn);

    if (strcmp(mode, "w") == 0) {
        conn->file = fopen(filename, "w");
    } else if (strcmp(mode, "r+") == 0) {
        conn->file = fopen(filename, "r+");

        if (conn->file) {
            Database_load(conn);
        }
    } else {
        die("Invalid file open mode", conn);
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
            conn->file = NULL;  // Prevent further access to file
        }
        if (conn->db) {
            free(conn->db->rows);
            free(conn->db);
            conn->db = NULL;    // Prevent further access to db
        }
        free(conn);
        conn = NULL;            // Prevent further access to conn
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        die("Failed to write database.", conn);

    rc = fflush(conn->file);
    if (rc == -1)
        die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for (i = 0; i < conn->db->max_rows; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if (addr->set)
        die("Already set, delete it first", conn);

    addr->set = 1;

    // Copy name and email
    char *res = strncpy(addr->name, name, conn->db->max_data - 1);
    if (!res)
        die("Name copy failed", conn);
    addr->name[conn->db->max_data - 1] = '\0';

    res = strncpy(addr->email, email, conn->db->max_data - 1);
    if (!res)
        die("Email copy failed", conn);
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
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for (i = 0; i < db->max_rows; i++) {
        struct Address *cur = &db->rows[i];

        if (cur->set) {
            Address_print(cur);
        }
    }
}

void Database_find(struct Connection *conn, const char *search_term)
{
    int i;
    struct Database *db = conn->db;

    for (i = 0; i < db->max_rows; i++) {
        struct Address *addr = &db->rows[i];

        if (addr->set && 
            (strstr(addr->name, search_term) || strstr(addr->email, search_term))) {
            Address_print(addr);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        die("USAGE: ex17 <dbfile> <action> [action params]", NULL);

    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = NULL;

    switch (action) {
        case 'c':
            if (argc != 5)
                die("USAGE: ex17 <dbfile> c <max_data> <max_rows>", NULL);

            int max_data = atoi(argv[3]);
            int max_rows = atoi(argv[4]);

            conn = Database_open(filename, "w", max_data, max_rows);  // Pass max_data and max_rows
            Database_create(conn);  // No need to pass max_data and max_rows here
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4)
                die("USAGE: ex17 <dbfile> g <id>", NULL);

            conn = Database_open(filename, "r+", 0, 0);  // Pass 0 for max_data and max_rows
            int id = atoi(argv[3]);
            if (id >= conn->db->max_rows)
                die("There's not that many records.", conn);

            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6)
                die("USAGE: ex17 <dbfile> s <id> <name> <email>", NULL);

            conn = Database_open(filename, "r+", 0, 0);  // Pass 0 for max_data and max_rows
            id = atoi(argv[3]);
            if (id >= conn->db->max_rows)
                die("There's not that many records.", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4)
                die("USAGE: ex17 <dbfile> d <id>", NULL);

            conn = Database_open(filename, "r+", 0, 0);  // Pass 0 for max_data and max_rows
            id = atoi(argv[3]);
            if (id >= conn->db->max_rows)
                die("There's not that many records.", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            conn = Database_open(filename, "r", 0, 0);  // Pass 0 for max_data and max_rows
            Database_list(conn);
            break;

        case 'f':
            if (argc != 4)
                die("USAGE: ex17 <dbfile> f <search_term>", NULL);

            conn = Database_open(filename, "r", 0, 0);  // Pass 0 for max_data and max_rows
            Database_find(conn, argv[3]);  // Ensure `Database_find` is declared
            break;

        default:
            die("Invalid action: c=create, g=get, s=set, d=del, l=list, f=find", conn);
    }

    Database_close(conn);
    return 0;
}
