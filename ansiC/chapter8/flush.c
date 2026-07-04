#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFSIZ 1024
#define OPEN_MAX 20
#define EOFVAL (-1)
#define PERMS 0666

enum myflags {
    MYREAD  = 01,
    MYWRITE = 02,
    MYUNBUF = 04,
    MYEOFF  = 010,
    MYERRR  = 020
};

typedef struct {
    int cnt;
    char *ptr;
    char *base;
    int flag;
    int fd;
} MYFILE;

MYFILE _iob[OPEN_MAX] = {
    { 0, NULL, NULL, MYREAD,  0 },
    { 0, NULL, NULL, MYWRITE, 1 },
    { 0, NULL, NULL, MYWRITE, 2 },
};

MYFILE *myfopen(char *name, char *mode)
{
    int fd;
    MYFILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a') {
        return NULL;
    }
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++) {
        if ((fp->flag & (MYREAD | MYWRITE)) == 0) {
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
    fp->flag = (*mode == 'r') ? MYREAD : MYWRITE;
    return fp;
}

int myflushbuf(int x, MYFILE *fp)
{
    unsigned nc;
    int bufsize;

    if (fp < _iob || fp >= _iob + OPEN_MAX) {
        return EOFVAL;
    }
    if ((fp->flag & (MYWRITE | MYERRR)) != MYWRITE) {
        return EOFVAL;
    }
    bufsize = (fp->flag & MYUNBUF) ? 1 : BUFSIZ;
    if (fp->base == NULL) {
        if ((fp->base = (char *) malloc(bufsize)) == NULL) {
            fp->flag |= MYERRR;
            return EOFVAL;
        }
    } else {
        nc = fp->ptr - fp->base;
        if (write(fp->fd, fp->base, nc) != nc) {
            fp->flag |= MYERRR;
            return EOFVAL;
        }
    }
    fp->ptr = fp->base;
    *fp->ptr++ = (char) x;
    fp->cnt = bufsize - 1;
    return x;
}

int myfflush(MYFILE *fp)
{
    int rc = 0;

    if (fp < _iob || fp >= _iob + OPEN_MAX) {
        return EOFVAL;
    }
    if (fp->flag & MYWRITE) {
        rc = myflushbuf(0, fp);
    }
    fp->ptr = fp->base;
    fp->cnt = (fp->flag & MYUNBUF) ? 1 : BUFSIZ;
    return rc;
}

int myfclose(MYFILE *fp)
{
    int rc;

    if ((rc = myfflush(fp)) != EOFVAL) {
        free(fp->base);
        fp->ptr = NULL;
        fp->cnt = 0;
        fp->base = NULL;
        fp->flag &= ~(MYREAD | MYWRITE);
    }
    return rc;
}

int main(void)
{
    MYFILE *fp;
    char *text = "Hello from myflushbuf!\n";

    fp = myfopen("output.txt", "w");
    if (fp == NULL) {
        write(2, "Error opening file\n", 19);
        return 1;
    }
    while (*text) {
        myflushbuf(*text++, fp);
    }
    myfclose(fp);
    write(1, "File written successfully.\n", 27);
    return 0;
}
