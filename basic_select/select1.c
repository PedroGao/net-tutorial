#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include "../lib/common.h"

#define MAX_LINE 1024

int main(int args, char **argv) {
    int sock_fd;
    sock_fd = make_tcp_client("127.0.0.1", 1024, true);
    if (sock_fd < 0) {
        perror("make tcp err.");
        exit(EXIT_FAILURE);
    }
    // set fd_set
    // read set
    fd_set readmask;
    // tmp set, for loop
    fd_set allreads;

    FD_ZERO(&allreads);
    // stdin
    FD_SET(STDIN_FILENO, &allreads);
    // sock
    FD_SET(sock_fd, &allreads);
    ssize_t n;
    char recv_line[MAX_LINE], send_line[MAX_LINE];
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        readmask = allreads;
        int rt = select(sock_fd + 1, &readmask, NULL, NULL, NULL);
        if (rt <= 0) {
            perror("select failed.");
        }
        if (FD_ISSET(sock_fd, &readmask)) {
            n = read(sock_fd, recv_line, MAX_LINE);
            if (n < 0) {
                perror("read err.");
            } else if (n == 0) {
                perror("server terminated \n");
            }
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }
        if (FD_ISSET(STDIN_FILENO, &readmask)) {
            if (fgets(send_line, MAX_LINE, stdin) != NULL) {
                printf("now sending %s\n", send_line);
                n = write(sock_fd, send_line, strlen(send_line));
                if (n < 0) {
                    perror("write failed.");
                }
                printf("send bytes: %zu \n", n);
            }
        }
    }
#pragma clang diagnostic pop
    close(sock_fd);
    return 0;
}