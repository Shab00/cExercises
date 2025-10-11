#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char *feeds[] = {
        "https://cointelegraph.com/rss/tag/bitcoin",
        "https://cointelegraph.com/rss/category/top-10-cryptocurrencies",
        "https://cointelegraph.com/rss/category/market-analysis",
        "https://cointelegraph.com/rss/tag/altcoin"
    };

    int times = sizeof(feeds) / sizeof(feeds[0]);
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <phrase>\n", argv[0]);
        return 1;
    }
    char *phrase = argv[1];
    int i;
    for (i = 0; i < times; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            char var[255];
            sprintf(var, "RSS_FEED=%s", feeds[i]);
            char *vars[] = {var, NULL};
            execle("/Users/bashaardhoot/miniforge3/bin/python", "python", "./rssgossip.py", phrase, NULL, vars);
            fprintf(stderr, "Can't run script: %s\n", strerror(errno));
            return 1;
        }
    }
    for (i = 0; i < times; i++) {
        wait(NULL);
    }
    return 0;
}
