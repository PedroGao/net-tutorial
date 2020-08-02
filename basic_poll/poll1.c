#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include "../lib/common.h"

#define QUEUE_SIZE 2
#define MAX_LINE 1024

int main(int args, char **argv) {
    int sock_fd;
    sock_fd = make_tcp_client("127.0.0.1", 1024, true);
    if (sock_fd < 0) {
        perror("make tcp client err.");
        exit(EXIT_FAILURE);
    }
    struct pollfd pfd[QUEUE_SIZE];
    pfd[0].fd = STDIN_FILENO;
    pfd[0].events = POLLIN;
    pfd[0].revents = 0;
    pfd[1].fd = sock_fd;
    pfd[1].events = POLLIN;
    pfd[1].revents = 0;

    ssize_t n;
    char recv_line[MAX_LINE], send_line[MAX_LINE];
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        int rt = poll(pfd, QUEUE_SIZE, -1);
        if (rt <= 0) {
            perror("poll failed.");
        }
        if (pfd[0].revents & POLLIN) {
            if (fgets(send_line, MAX_LINE, stdin) != NULL) {
                printf("now sending %s\n", send_line);
                n = write(sock_fd, send_line, strlen(send_line));
                if (n < 0) {
                    perror("write failed.");
                }
                printf("send bytes: %zu \n", n);
            }
        }
        if (pfd[1].revents & POLLIN) {
            n = read(sock_fd, recv_line, MAX_LINE);
            if (n < 0) {
                perror("read err.");
            } else if (n == 0) {
                perror("server terminated \n");
            }
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }
    }
#pragma clang diagnostic pop
    return 0;
}