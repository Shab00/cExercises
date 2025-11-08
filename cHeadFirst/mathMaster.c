#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

static volatile sig_atomic_t time_up = 0;
static volatile sig_atomic_t user_quit = 0;
static int score = 0;

void times_up(int sig)
{
    const char msg[] = "\nTIME'S UP!\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    time_up = 1;
}

void end_game(int sig)
{
    user_quit = 1;
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

void error(const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    const unsigned int TIME_LIMIT_SECONDS = 30;

    if (catch_signal(SIGALRM, times_up) == -1)
        error("Can't install SIGALRM handler");
    if (catch_signal(SIGINT, end_game) == -1)
        error("Can't install SIGINT handler");

    alarm(TIME_LIMIT_SECONDS);

    srandom((unsigned)time(NULL));

    char buf[32];
    while (!time_up && !user_quit) {
        int a = random() % 11;
        int b = random() % 11;

        printf("\nWhat is %d times %d? ", a, b);
        fflush(stdout);

        if (fgets(buf, sizeof(buf), stdin) == NULL) {
            if (time_up || user_quit) break;

            if (feof(stdin)) break;

            if (ferror(stdin)) {
                if (errno == EINTR) {
                    clearerr(stdin);
                    continue;
                } else {
                    error("Error reading input");
                }
            }
            break;
        }

        buf[strcspn(buf, "\n")] = '\0';

        if (buf[0] == '\0') continue;

        if (strcmp(buf, "q") == 0 || strcmp(buf, "Q") == 0) {
            user_quit = 1;
            break;
        }

        int answer = atoi(buf);
        if (answer == a * b) {
            score++;
            printf("Correct! Score: %d\n", score);
        } else {
            printf("Wrong! %d x %d = %d  (Score: %d)\n", a, b, a * b, score);
        }
    }

    if (time_up) {
        printf("\nTime's up! Final score: %d\n", score);
    } else if (user_quit) {
        printf("\nInterrupted by user. Final score: %d\n", score);
    } else {
        printf("\nGoodbye. Final score: %d\n", score);
    }

    return 0;
}
