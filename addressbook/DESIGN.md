# Address Book — Design

## 1. What this is

A single-binary C program that manages a local collection of contacts.
The address book lives in memory while the program runs, is saved to a
binary file on disk, and is loaded back on startup.

Scope: add, list, search by name, delete, save, load.
Interface: interactive terminal menu.

## 2. Core data types

### Contact
A single person. All fields are fixed-size character arrays — no malloc,
no pointers, no variable-length data. Every Contact is the same size.

- `name[50]`     — null-terminated, up to 49 chars
- `phone[15]`    — null-terminated, up to 14 chars
- `email[50]`    — null-terminated, up to 49 chars (optional field)

### AddressBook
The in-memory database.

- `list[100]`    — fixed array of Contacts
- `count`        — int, number of valid entries (0..100)

**Why fixed capacity:** avoids dynamic allocation and reallocation while
learning the file-format side. If we ever outgrow 100, we change the
struct and the file format together. This is a v1 limitation, not a
final design.

## 3. Layering: data vs. UI

This is the most important structural rule in the project:

> **Data functions do not do I/O. The UI layer collects input, calls
> data functions, and displays results.**

The data layer is pure: it takes a Contact or an index, manipulates the
in-memory array, and returns a status. It never prints, never prompts,
never reads stdin. This means it can be tested without a terminal, and
later swapped to a CLI-subcommand interface without rewriting the logic.

### Data layer (in addressbook.c)

- `void book_init(AddressBook *book)`
  Zero the count, no other state needed.

- `int contact_add(AddressBook *book, const Contact *c)`
  Copy `*c` into `book->list[book->count]`, increment count.
  Returns 0 on success, -1 if full.

- `int contact_search_by_name(const AddressBook *book, const char *name)`
  Linear scan, `strcmp` on `list[i].name`.
  Returns index of first match, or -1 if not found.
  **Does not print.**

- `int contact_delete(AddressBook *book, int index)`
  Shift elements [index+1..count-1] left by one, decrement count.
  Returns 0 on success, -1 if index out of range.

- `void contact_list_all(const AddressBook *book)`
  See note in §3a — this one is borderline. See below.

### UI layer (in main.c, or a separate ui.c if main grows)

- `void ui_show_menu(void)` — prints the options.
- `void ui_add_contact(AddressBook *book)` — prompts, reads with fgets,
  strips newline, fills a local Contact, calls `contact_add`, prints result.
- `void ui_search_contact(const AddressBook *book)` — prompts for name,
  calls `contact_search_by_name`, prints the match if index >= 0,
  prints "not found" otherwise.
- `void ui_delete_contact(AddressBook *book)` — prompts for name,
  calls search, then delete, prints result.
- `void ui_list_contacts(const AddressBook *book)` — calls the loop
  that prints each record.

**3a. About `contact_list_all`:** printing is UI work. Two clean options:

(a) The data layer exposes `const Contact *contact_get(const AddressBook *, int index)` and the UI loops over count, printing each. **Recommended** — keeps data layer 100% I/O-free.
(b) The data layer has `contact_list_all` that prints. Simpler, but violates the rule.

Pick (a) if you want the discipline. Pick (b) if you want to move faster and accept the exception. Note whichever you pick in this doc.

## 4. File format

**Header (16 bytes):**
- Magic: 4 bytes `"ADDR"` — reject any file not starting with this
- Version: 1 byte (0x01) — for future format changes
- Record size: 1 byte (sizeof(Contact) — sanity check on load)
- Count: 4 bytes (uint32, little-endian)
- Reserved: 6 bytes (zero)

**Records:** for each contact, write the three fields one at a time
with `fwrite` of `sizeof(field)` bytes each.

This is explicit and documented. Not portable across endianness either
(without byte-swapping the count), but the *fields* are safe from struct
padding, and the format is written down. If you change `Contact`, you
bump the version byte and handle it on load.

**Why a header at all:** a magic value lets `load` reject a wrong file
(a text file, someone else's binary, a corrupt file) with a clean error
instead of loading garbage into your array. A count field means you know
how many records to expect before reading any. A version field means you
can change the format later without losing compatibility.

**A note on endianness:** the count in the header (4 bytes) and the
record fields (all char arrays, so no issue) — the count is the only
multi-byte integer. For v1, write it native and note the limitation.
Byte-swapping can wait for a later exercise.

## 5. Error handling

- All system actions return a status; nothing calls `exit()` mid-program.
- Every file operation (`fopen`, `fread`, `fwrite`, `fclose`) has its
  return value checked.
- A missing address book file on startup is **not** an error — it just
  means the user hasn't saved anything yet. Print a note to stderr,
  initialize an empty book, and continue.
- A file that opens but has a bad magic value or wrong record size is
  an error — refuse to load, print to stderr, continue with empty book.
- Corrupt or partially-read files: if `fread` returns fewer records
  than the header's count, treat the file as corrupt. Don't load a
  partial file as if it were complete.
- Return codes for v1: `0` for success, `-1` for any failure. v2 could
  distinguish failure modes; out of scope here.

## 6. Input handling rules (non-negotiable)

- **Never use `gets`.** It cannot be made safe.
- **Always `fgets(buf, sizeof(buf), stdin)`.** Never assumes buffer size.
- **Always strip trailing newline** after fgets using `strcspn`.
- **On overflow** (fgets read a full buffer but no newline): discard the
  rest of the line before the next prompt, so the next input isn't
  contaminated by leftover characters.
- **Reject empty names** (after stripping whitespace, name length 0).

## 7. Entry point: the menu loop

Main does:

1. Declare an `AddressBook` on the stack.
2. `book_init(&book)`.
3. Try to `storage_load_from_file(&book, "addressbook.db")` —
   missing file is fine.
4. Loop:
   - `ui_show_menu()`
   - Read a choice from stdin
   - Dispatch to the matching `ui_*` function
   - Loop until the user chooses "quit"
5. On quit, offer to save. If yes, `storage_save_to_file(&book, "addressbook.db")`.
6. Return 0.

The menu loop is the *only* place the program is interactive. Everything
under it is either UI helpers or the pure data layer.

## 8. v1 Limitations (deliberate)

- Fixed capacity 100. No dynamic growth.
- Fixed field sizes (name 50, phone 15, email 50).
- No update/edit operation. Add + delete only.
- No sorting. List returns insertion order.
- Endianness of the count field is native, not portable.
- No concurrent access, no locking.
- No encryption, no password protection.
- File format is not human-readable.

These are decisions, not oversights. Each can be a v2 exercise.

## 9. File layout on disk
addressbook/
├── include/
│ └── addressbook.h
├── src/
│ ├── addressbook.c
│ └── main.c
├── Makefile
├── DESIGN.md ← this file
└── .gitignore ← ignore *.o, bin/, lib/, *.db
