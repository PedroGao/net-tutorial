#include <unistd.h>
#include <stdlib.h>
#include "../lib/common.h"

#define MAX_LEN 1000

int main(int argc, char const *argv[]) {
    int listen_fd;
    listen_fd = make_tcp_server("127.0.0.1", 1024, 10, true);
    if (listen_fd < 0) {
        perror("make tcp server err.");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in client_addr;
    socklen_t client_len;
    int conn_fd;
    client_len = sizeof(client_addr);
    conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
    char buffer[MAX_LEN];
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(conn_fd, buffer, MAX_LEN);
        if (n <= 0) {
            break;
        }
        fputs(buffer, stdout);
        write(conn_fd, buffer, n);
    }
#pragma clang diagnostic pop
    close(conn_fd);
    close(listen_fd);
    return 0;
}
