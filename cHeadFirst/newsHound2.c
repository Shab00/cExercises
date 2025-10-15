#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <phrase>\n", argv[0]);
        return 1;
    }

    char *phrase = argv[1];
    char *vars[] = {"RSS_FEED=https://cointelegraph.com/rss/tag/bitcoin", NULL};
    FILE *f = fopen("stories.txt", "w");
    if (!f) {
        error("Can't open stories.txt");
    }

    pid_t pid = fork();
    if (pid == -1) {
        error ("Can't fork process");
    }

    if (pid == 0) {
        if (dup2(fileno(f), 1) == -1) {
            error("Can't redirect Standard Output");
        }
        fclose(f);

        execle("/Users/bashaardhoot/miniforge3/bin/python", "python", "./rssgossip.py", phrase, NULL, vars);
        error("Can't run script");
    } else {
        fclose(f);
        wait(NULL);
    }
    return 0;
}
