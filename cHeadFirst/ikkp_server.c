#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

int listener_d = 0;

void handle_shutdown(int sig);
void handle_sigchld(int sig);
int catch_signal(int sig, void (*handler)(int));
int open_listener_socket(void);
void bind_to_port(int socket, int port);
int say(int socket, const char *s);
int read_in(int socket, char *buf, int size);
void error(const char *msg);

void handle_shutdown(int sig)
{
    if (listener_d) {
        close(listener_d);
        listener_d = 0;
    }
    const char bye[] = "\nBye!\n";
    write(STDERR_FILENO, bye, sizeof(bye) - 1);
    _exit(0);
}

void handle_sigchld(int sig)
{
    (void)sig;
    while (1) {
        int status;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        if (pid <= 0)
            break;
    }
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    return sigaction(sig, &action, NULL);
}

int open_listener_socket(void)
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1)
        error("Can't create socket");

    int reuse = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const void *)&reuse, sizeof(reuse)) == -1) {
        close(s);
        error("Can't set SO_REUSEADDR");
    }

    return s;
}

void bind_to_port(int socket, int port)
{
    struct sockaddr_in name;
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons((in_port_t)port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket, (struct sockaddr *)&name, sizeof(name)) == -1)
        error("Can't bind the port");
}

int say(int socket, const char *s)
{
    ssize_t len = (ssize_t)strlen(s);
    ssize_t sent = send(socket, s, len, 0);
    if (sent == -1)
        perror("send");
    return (int)sent;
}

int read_in(int socket, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    ssize_t n;

    while (i < size - 1 && (n = recv(socket, &c, 1, 0)) == 1) {
        if (c == '\r')
            continue;
        if (c == '\n')
            break;
        buf[i++] = c;
    }

    if (n == -1) {
        if (errno == EINTR)
            return -1;
        perror("recv");
        return -1;
    }

    if (n == 0 && i == 0)
        return -1;

    buf[i] = '\0';
    return i;
}

void error(const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(int argc, char *argv[])
{
    if (catch_signal(SIGINT, handle_shutdown) == -1)
        error("Can't set the interrupt handler");

    if (catch_signal(SIGCHLD, handle_sigchld) == -1)
        error("Can't set SIGCHLD handler");

    listener_d = open_listener_socket();
    bind_to_port(listener_d, 30000);

    if (listen(listener_d, 10) == -1)
        error("Can't listen");

    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    char buf[255];

    puts("Waiting for connection");

    while (1) {
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        if (connect_d == -1) {
            if (errno == EINTR)
                continue;
            perror("accept");
            continue;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            close(connect_d);
            continue;
        }

        if (pid == 0) {
            close(listener_d);

            if (say(connect_d,
                    "Internet Knock-Knock Protocol Server\r\nVersion 1.0\r\nKnock! Knock!\r\n> ")
                != -1) {
                if (read_in(connect_d, buf, sizeof(buf)) == -1) {
                    close(connect_d);
                    _exit(0);
                }

                if (strncasecmp("Who's there?", buf, 12) != 0) {
                    say(connect_d, "You should say 'Who's there?'!");
                } else {
                    if (say(connect_d, "Oscar\r\n> ") != -1) {
                        if (read_in(connect_d, buf, sizeof(buf)) == -1) {
                            close(connect_d);
                            _exit(0);
                        }

                        if (strncasecmp("Oscar who?", buf, 10) != 0)
                            say(connect_d, "You should say 'Oscar who?'!\r\n");
                        else
                            say(connect_d, "Oscar silly question, you get a silly answer\r\n");
                    }
                }
            }

            close(connect_d);
            _exit(0);
        }

        close(connect_d);
    }

    close(listener_d);
    return 0;
}
