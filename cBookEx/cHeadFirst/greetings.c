#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void diediedie(int sig)
{
    puts("Goodbye cruel world....");
    exit(1);
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0; 
    return sigaction(sig, &action, NULL);
}

int main(void)
{
    if (catch_signal(SIGINT, diediedie) == -1) {
        fprintf(stderr, "Can't map the handler\n");
        exit(2);
    }

    char name[30];
    printf("Enter your name: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        fprintf(stderr, "No input or interrupted\n");
        return 1;
    }
    name[strcspn(name, "\n")] = '\0';
    printf("Hello %s\n", name);
    return 0;
}
