#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define PERMS 0666
#define OPEN_MAX 20
#define BUFSIZ 1024
#define MYEOF (-1)

typedef struct {
    unsigned is_read  : 1;
    unsigned is_write : 1;
    unsigned is_unbuf : 1;
    unsigned is_buf   : 1;
    unsigned is_eof   : 1;
    unsigned is_err   : 1;
} flag_field;

typedef struct {
    int cnt;
    char *ptr;
    char *base;
    flag_field flag;
    int fd;
} MYFILE;

MYFILE _iob[OPEN_MAX] = {
    { 0, NULL, NULL, {1, 0, 0, 0, 0, 0}, 0 },
    { 0, NULL, NULL, {0, 1, 0, 0, 0, 0}, 1 },
    { 0, NULL, NULL, {0, 1, 0, 0, 0, 0}, 2 },
};

MYFILE *myfopen(char *name, char *mode)
{
    int fd;
    MYFILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a') {
        return NULL;
    }
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++) {
        if (fp->flag.is_read == 0 && fp->flag.is_write == 0) {
            break;
        }
    }
    if (fp >= _iob + OPEN_MAX) {
        return NULL;
    }
    if (*mode == 'w') {
        fd = creat(name, PERMS);
    } else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1) {
            fd = creat(name, PERMS);
        }
        lseek(fd, 0L, 2);
    } else {
        fd = open(name, O_RDONLY, 0);
    }
    if (fd == -1) {
        return NULL;
    }
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag.is_unbuf = 0;
    fp->flag.is_buf   = 1;
    fp->flag.is_eof   = 0;
    fp->flag.is_err   = 0;
    if (*mode == 'r') {
        fp->flag.is_read  = 1;
        fp->flag.is_write = 0;
    } else {
        fp->flag.is_read  = 0;
        fp->flag.is_write = 1;
    }
    return fp;
}

int myfillbuf(MYFILE *fp)
{
    int bufsize;

    if (fp->flag.is_read == 0 ||
        fp->flag.is_eof  == 1 ||
        fp->flag.is_err  == 1) {
        return MYEOF;
    }
    bufsize = (fp->flag.is_unbuf == 1) ? 1 : BUFSIZ;
    if (fp->base == NULL) {
        if ((fp->base = (char *) malloc(bufsize)) == NULL) {
            return MYEOF;
        }
    }
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1) {
            fp->flag.is_eof = 1;
        } else {
            fp->flag.is_err = 1;
        }
        fp->cnt = 0;
        return MYEOF;
    }
    return (unsigned char) *fp->ptr++;
}

void myputchar(int c)
{
    char ch = (char) c;
    write(1, &ch, 1);
}

void myprint(char *s)
{
    while (*s) {
        myputchar(*s++);
    }
}

void myprintln(char *s)
{
    myprint(s);
    myputchar('\n');
}

int main(void)
{
    MYFILE *fp;
    int c;
    char *text = "Hello from bit fields!\n";

    fp = myfopen("test.txt", "w");
    if (fp != NULL) {
        write(fp->fd, text, 22);
        close(fp->fd);
    }

    fp = myfopen("test.txt", "r");
    if (fp == NULL) {
        write(2, "Error opening file\n", 19);
        return 1;
    }
    myprintln("Reading file with myfillbuf:");
    while ((c = myfillbuf(fp)) != MYEOF) {
        myputchar(c);
    }
    return 0;
}
