#include <stdio.h>
#include <string.h>
#include "dbg.h"

int normal_copy(char *from, char *to, int count)
{
    int i = 0;

    for (i = 0; i < count; i++) {
        to[i] = from[i];
    }

    return i;
}

int duffs_device(char *from, char *to, int count)
{
    {
        int n = (count + 7) / 8;

        switch (count % 8) {
            case 0:
                do {
                    *to++ = *from++;
                    case 7:
                    *to++ = *from++;
                    case 6:
                    *to++ = *from++;
                    case 5:
                    *to++ = *from++;
                    case 4:
                    *to++ = *from++;
                    case 3:
                    *to++ = *from++;
                    case 2:
                    *to++ = *from++;
                    case 1:
                    *to++ = *from++;
                } while (--n > 0);
        }
    }

    return count;
}

int zeds_device(char *from, char *to, int count)
{
    {
        int n = (count + 7) / 8;

        switch (count % 8) {
            case 0:
again:    *to++ = *from++;

            case 7:
          *to++ = *from++;
            case 6:
          *to++ = *from++;
            case 5:
          *to++ = *from++;
            case 4:
          *to++ = *from++;
            case 3:
          *to++ = *from++;
            case 2:
          *to++ = *from++;
            case 1:
          *to++ = *from++;
          if (--n > 0)
              goto again;
        }
    }

    return count;
}

#define GENERATE_DUFFS_DEVICE(UNROLL_FACTOR) \
void duffs_device_##UNROLL_FACTOR(char *to, char *from, int count) { \
    int n = (count + (UNROLL_FACTOR - 1)) / UNROLL_FACTOR; \
    switch (count % UNROLL_FACTOR) { \
        case 0: do { *to++ = *from++; \
        case UNROLL_FACTOR - 1: *to++ = *from++; \
        case UNROLL_FACTOR - 2: *to++ = *from++; \
        case UNROLL_FACTOR - 3: *to++ = *from++; \
        case UNROLL_FACTOR - 4: *to++ = *from++; \
        case UNROLL_FACTOR - 5: *to++ = *from++; \
        case UNROLL_FACTOR - 6: *to++ = *from++; \
        case UNROLL_FACTOR - 7: *to++ = *from++; \
        /* Add more cases if UNROLL_FACTOR > 8 */ \
        } while (--n > 0); \
    } \
}

// Generate a Duff's device-like function with an unroll factor of 8
GENERATE_DUFFS_DEVICE(8)

// Generate a Duff's device-like function with an unroll factor of 16
GENERATE_DUFFS_DEVICE(16)

int valid_copy(char *data, int count, char expects)
{
    int i = 0;
    for (i = 0; i < count; i++) {
        if (data[i] != expects) {
            log_err("[%d] %c != %c", i, data[i], expects);
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char from[1000] = { 'a' };
    char to[1000] = { 'c' };
    int rc = 0;

    // set up the from to have some stuff
    memset(from, 'x', 1000);
    // set it to failure mode
    memset(to, 'y', 1000);
    check(valid_copy(to, 1000, 'y'), "Not initialized right.");

    // use normal copy to
    rc = normal_copy(from, to, 1000);
    check(rc == 1000, "Normal copy failed: %d", rc);
    check(valid_copy(to, 1000, 'x'), "Normal copy failed.");

    //reset
    memset(to, 'y', 1000);

    // duffs version
    rc = duffs_device(from, to, 1000);
    check(rc == 1000, "Duff's device failed: %d", rc);
    check(valid_copy(to, 1000, 'x'), "Duff's device failed copy.");
    
    //reset
    memset(to, 'y', 1000);
    
    // my version
    rc = zeds_device(from, to, 1000);
    check(rc == 1000, "Zed's device failed: %d", rc);
    check(valid_copy(to, 1000, 'x'), "Zed's device failed copy.");

    // Initialize the source buffer with 'x'
    memset(from, 'x', 1000);

    // Test Duff's device with unroll factor of 8
    memset(to, 'y', 1000);
    duffs_device_8(from, to, 1000);
    printf("Duff's device (unroll 8): %s\n", memcmp(to, from, 1000) == 0 ? "Success" : "Failure");

    // Test Duff's device with unroll factor of 16
    memset(to, 'y', 1000);
    duffs_device_16(from, to, 1000);
    printf("Duff's device (unroll 16): %s\n", memcmp(to, from, 1000) == 0 ? "Success" : "Failure");

    return 0;
error:
    return 1;
}
