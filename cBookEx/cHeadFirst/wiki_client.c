#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

void error(const char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int open_socket(const char *host, const char *port)
{
    struct addrinfo *res;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int gai = getaddrinfo(host, port, &hints, &res);
    if (gai != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai == EAI_SYSTEM ? strerror(errno) : gai_strerror(gai));
        exit(1);
    }

    int d_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (d_sock == -1)
        error("Can't open socket");

    if (connect(d_sock, res->ai_addr, res->ai_addrlen) == -1) {
        freeaddrinfo(res);
        close(d_sock);
        error("Can't connect to socket");
    }

    freeaddrinfo(res);
    return d_sock;
}

int say(int socket_fd, const char *s)
{
    size_t total = 0;
    size_t len = strlen(s);
    while (total < len) {
        ssize_t sent = send(socket_fd, s + total, len - total, 0);
        if (sent == -1) {
            fprintf(stderr, "Error talking to the server: %s\n", strerror(errno));
            return -1;
        }
        total += (size_t)sent;
    }
    return (int)total;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <Wikipedia-page-slug>\n", argv[0]);
        fprintf(stderr, "Example: %s Bitcoin\n", argv[0]);
        return 1;
    }

    char req[1024];
    int n = snprintf(req, sizeof(req),
                     "GET /wiki/%s HTTP/1.1\r\n"
                     "Host: en.wikipedia.org\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     argv[1]);
    if (n < 0 || (size_t)n >= sizeof(req)) {
        fprintf(stderr, "Request too long\n");
        return 1;
    }

    int d_sock = open_socket("en.wikipedia.org", "80");

    if (say(d_sock, req) == -1) {
        close(d_sock);
        return 1;
    }

    char rec[4096];
    ssize_t bytesRcvd = recv(d_sock, rec, sizeof(rec) - 1, 0);
    while (bytesRcvd > 0) {
        rec[bytesRcvd] = '\0';
        fputs(rec, stdout);
        bytesRcvd = recv(d_sock, rec, sizeof(rec) - 1, 0);
    }
    if (bytesRcvd == -1)
        error("Can't read from server");

    close(d_sock);
    return 0;
}
