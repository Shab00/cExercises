#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

void open_url(const char *url) {
    char command[512];
    snprintf(command, sizeof(command), "open \"%s\"", url);
    system(command);
}

int main(int argc, char *argv[])
{
    char *phrase = argv[1];
    char *vars[] = {"RSS_FEED=https://cointelegraph.com/rss/tag/bitcoin", NULL};

    int fd[2];
    if (pipe(fd) == -1) {
        error("Can't create pipe");
    }
    pid_t pid = fork();
    if (pid == -1) {
        error("Can't fork process");
    }
    if (pid == 0) {
        dup2(fd[1], 1);
        close(fd[0]);
        if (execle("/Users/bashaardhoot/miniforge3/bin/python", "python", "./rssgossip.py", "-u", phrase, NULL, vars) == -1) {
            error("Can't run script");
        }
    }
    dup2(fd[0], 0);
    close(fd[1]);
    char line[255];
    while (fgets(line, 255, stdin)) {
        if (line[0] == '\t')
            open_url(line + 1);
    }
    return 0;
}
