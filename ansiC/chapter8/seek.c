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

int myfseek(MYFILE *fp, long offset, int origin)
{
    unsigned nc;
    long rc = 0;

    if (fp->flag & MYREAD) {
        if (origin == 1) {
            offset -= fp->cnt;
        }
        rc = lseek(fp->fd, offset, origin);
        fp->cnt = 0;
        fp->ptr = fp->base;
    } else if (fp->flag & MYWRITE) {
        if ((nc = fp->ptr - fp->base) > 0) {
            if (write(fp->fd, fp->base, nc) != nc) {
                rc = -1;
            }
        }
        if (rc != -1) {
            rc = lseek(fp->fd, offset, origin);
        }
        fp->ptr = fp->base;
        fp->cnt = (fp->flag & MYUNBUF) ? 1 : BUFSIZ;
    }
    return (rc == -1) ? -1 : 0;
}

int main(void)
{
    int fd;
    char buf[20];
    int n;

    fd = creat("seektest.txt", PERMS);
    write(fd, "0123456789ABCDEFGHIJ", 20);
    close(fd);

    write(1, "File contents: 0123456789ABCDEFGHIJ\n", 36);

    write(1, "\nSeek 10 from start, read 5: ", 29);
    fd = open("seektest.txt", O_RDONLY);
    lseek(fd, 10, 0);
    n = read(fd, buf, 5);
    buf[n] = '\0';
    write(1, buf, 5);
    write(1, "\n", 1);
    close(fd);

    write(1, "Seek 5 from start, then +3, read 4: ", 36);
    fd = open("seektest.txt", O_RDONLY);
    lseek(fd, 5, 0);
    lseek(fd, 3, 1);
    n = read(fd, buf, 4);
    buf[n] = '\0';
    write(1, buf, 4);
    write(1, "\n", 1);
    close(fd);

    write(1, "Seek -5 from end, read 5: ", 26);
    fd = open("seektest.txt", O_RDONLY);
    lseek(fd, -5, 2);
    n = read(fd, buf, 5);
    buf[n] = '\0';
    write(1, buf, 5);
    write(1, "\n", 1);
    close(fd);

    return 0;
}
