#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h> 

void error(const char *message) {
    fprintf(stderr, "Error: %s: %s\n", message, strerror(errno));
    exit(1);
}

void open_url(const char *url) {
    char command[512];
    char clean_url[255];
    strcpy(clean_url, url);
    char *newline = strchr(clean_url, '\n');
    if (newline) *newline = '\0';
    
    snprintf(command, sizeof(command), "open %s", clean_url);

    if (system(command) == -1) {
        fprintf(stderr, "Warning: Could not open URL: %s\n", clean_url);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s search_phrase\n", argv[0]);
        exit(1);
    }

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
    
    if (!pid) {
        close(fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            error("Can't redirect stdout");
        }
        close(fd[1]);
        if (execle("/Users/bashaardhoot/miniforge3/bin/python", 
                    "python", 
                    "./rssgossip.py",
                    "-u", 
                    phrase, 
                    NULL, 
                    vars) == -1) {
            error("Can't run script");
        }
        exit(1);
    }
    
    close(fd[1]);
    
    FILE* in = fdopen(fd[0], "r");
    if (!in) {
        error("Can't open pipe for reading");
    }
    
    char line[255];
    while (fgets(line, 255, in)) {
        if (line[0] == '\t') {
            open_url(line + 1);
        } else {
            printf("%s", line);
        }
    }
    
    fclose(in);
    
    int status;
    waitpid(pid, &status, 0);
    
    return 0;
}
