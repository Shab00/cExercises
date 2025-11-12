#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(void)
{
    const char *advice[] = {
        "Take smaller bites\r\n",
        "Go for the tight jeans. No they do NOT make you look fat.\r\n",
        "One word: inappropriate\r\n",
        "Just for today, be honest. Tell your boss what you *really* think\r\n",
        "You might want to rethink that haircut\r\n"
    };
    const size_t advice_count = sizeof(advice) / sizeof(advice[0]);

    srand((unsigned)time(NULL));

    int listener_d = socket(AF_INET, SOCK_STREAM, 0);
    if (listener_d == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(listener_d);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in name;  
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(30000);
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener_d, (struct sockaddr *)&name, sizeof(name)) == -1) {
        perror("bind");
        close(listener_d);
        exit(EXIT_FAILURE);
    }

    if (listen(listener_d, 10) == -1) {
        perror("listen");
        close(listener_d);
        exit(EXIT_FAILURE);
    }

    puts("Waiting for connection on port 30000...");

    struct sockaddr_storage client_addr;
    socklen_t address_size = sizeof(client_addr);
    int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
    if (connect_d == -1) {
        perror("accept");
        close(listener_d);
        exit(EXIT_FAILURE);
    }

    const char *msg = advice[rand() % advice_count];
    ssize_t sent = send(connect_d, msg, strlen(msg), 0);
    if (sent == -1) {
        perror("send");
    }

    close(connect_d);
    close(listener_d);

    return 0;
}
