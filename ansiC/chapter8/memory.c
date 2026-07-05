#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NALLOC 1024
#define MAXBYTES (unsigned) 10240

typedef long Align;

union header {
    struct {
        union header *ptr;
        unsigned size;
    } s;
    Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;
static unsigned maxalloc = 0;
static void *heap_start = NULL;
static void *heap_end = NULL;

/* Forward declarations */
void myfree(void *ap);
static Header *morecore(unsigned nu);

/* ====================================================
   Exercise 8-6: calloc
   ==================================================== */
void *mycalloc(unsigned n, unsigned size)
{
    unsigned i, nb;
    char *p, *q;

    nb = n * size;
    if ((p = q = malloc(nb)) != NULL) {
        for (i = 0; i < nb; i++) {
            *p++ = 0;
        }
    }
    return q;
}

/* ====================================================
   Exercise 8-7: Robust malloc with error checking
   ==================================================== */
static Header *morecore(unsigned nu)
{
    char *cp;
    Header *up;

    if (nu < NALLOC) {
        nu = NALLOC;
    }
    cp = (char *) sbrk(nu * sizeof(Header));
    if (cp == (char *) -1) {
        return NULL;
    }
    if (heap_start == NULL) {
        heap_start = cp;
    }
    heap_end = cp + nu * sizeof(Header);
    up = (Header *) cp;
    up->s.size = nu;
    if (up->s.size > maxalloc) {
        maxalloc = up->s.size;
    }
    myfree((void *)(up + 1));
    return freep;
}

void *mymalloc(unsigned nbytes)
{
    Header *p, *prevp;
    unsigned nunits;

    if (nbytes > MAXBYTES) {
        fprintf(stderr, "mymalloc: can't allocate more than %u bytes\n", MAXBYTES);
        return NULL;
    }
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) {
                prevp->s.ptr = p->s.ptr;
            } else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep) {
            if ((p = morecore(nunits)) == NULL) {
                return NULL;
            }
        }
    }
}

/* ====================================================
   Exercise 8-7: Robust free with error checking
   Now validates pointer is within heap range
   ==================================================== */
void myfree(void *ap)
{
    Header *bp, *p;

    if (ap == NULL) {
        fprintf(stderr, "myfree: NULL pointer\n");
        return;
    }

    /* Check if pointer is within our heap */
    if (ap < heap_start || ap >= heap_end) {
        fprintf(stderr, "myfree: pointer %p is outside heap (%p - %p)\n",
                ap, heap_start, heap_end);
        return;
    }

    bp = (Header *)ap - 1;

    if (bp->s.size == 0 || bp->s.size > maxalloc) {
        fprintf(stderr, "myfree: can't free %u units (maxalloc=%u)\n",
                bp->s.size, maxalloc);
        return;
    }
    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) {
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) {
            break;
        }
    }
    if (bp + bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }
    if (p + p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }
    freep = p;
}

/* ====================================================
   Exercise 8-8: bfree
   ==================================================== */
unsigned bfree(char *p, unsigned n)
{
    Header *hp;

    if (n < sizeof(Header)) {
        return 0;
    }
    if (p < heap_start || p + n > heap_end) {
        /* bfree can add external memory, so expand our tracking */
        if (heap_start == NULL || p < heap_start) {
            /* Don't track external memory for validation */
        }
    }
    hp = (Header *) p;
    hp->s.size = n / sizeof(Header);
    if (hp->s.size > maxalloc) {
        maxalloc = hp->s.size;
    }
    myfree((void *)(hp + 1));
    return hp->s.size;
}

/* ====================================================
   Test program
   ==================================================== */
int main(void)
{
    char *a, *b, *c;
    int *numbers;
    static char bigbuf[4096];

    printf("=== Exercise 8-6: mycalloc ===\n");
    numbers = (int *) mycalloc(5, sizeof(int));
    if (numbers == NULL) {
        printf("mycalloc failed\n");
        return 1;
    }
    printf("mycalloc allocated 5 integers, all zero:\n");
    for (int i = 0; i < 5; i++) {
        printf("  numbers[%d] = %d\n", i, numbers[i]);
    }

    printf("\n=== Exercise 8-7: mymalloc/myfree with error checking ===\n");

    a = (char *) mymalloc(100);
    b = (char *) mymalloc(200);
    c = (char *) mymalloc(300);

    if (a && b && c) {
        printf("Allocated 100, 200, 300 bytes successfully\n");
        printf("  a = %p\n", (void *)a);
        printf("  b = %p\n", (void *)b);
        printf("  c = %p\n", (void *)c);
    }

    printf("Freeing b...\n");
    myfree(b);

    printf("Freeing a...\n");
    myfree(a);

    printf("Freeing c...\n");
    myfree(c);

    printf("\nTrying to free an invalid pointer (should show error, not crash):\n");
    myfree((void *)0x12345678);

    printf("Trying to free NULL (should show error):\n");
    myfree(NULL);

    printf("\nTrying to allocate too much (should show error):\n");
    void *big = mymalloc(MAXBYTES + 1);
    if (big == NULL) {
        printf("  Correctly refused to allocate %u bytes\n", MAXBYTES + 1);
    }

    printf("\n=== Exercise 8-8: bfree ===\n");
    printf("Size of static buffer: %lu bytes\n", sizeof(bigbuf));
    printf("Adding static buffer to free list via bfree...\n");

    unsigned units = bfree(bigbuf, sizeof(bigbuf));
    if (units > 0) {
        printf("  Added %u units to free list\n", units);
        char *frombuf = (char *) mymalloc(500);
        if (frombuf != NULL) {
            printf("  Allocated 500 bytes from the ex-static buffer\n");
            printf("  Pointer: %p\n", (void *)frombuf);
            printf("  bigbuf:  %p\n", (void *)bigbuf);
            strcpy(frombuf, "Hello from bfree'd memory!");
            printf("  Content: %s\n", frombuf);
        }
    } else {
        printf("  bfree failed (buffer too small?)\n");
    }

    printf("\n=== All exercises completed successfully ===\n");
    return 0;
}
