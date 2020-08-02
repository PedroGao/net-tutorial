#include <unistd.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <errno.h>
#include "../lib/common.h"

#define QUEUE_SIZE 10
#define MAX_LINE 1024

int main(int args, char **argv) {
    int listen_fd;
    listen_fd = make_tcp_server("127.0.0.1", 1024, 10, true);
    if (listen_fd < 0) {
        perror("make tcp client err.");
        exit(EXIT_FAILURE);
    }
    struct pollfd event_set[QUEUE_SIZE];
    event_set[0].fd = listen_fd;
    event_set[0].events = POLLRDNORM;

    int i;
    for (i = 1; i < QUEUE_SIZE; ++i) {
        // 如果 fd = -1，则会忽略
        event_set[i].fd = -1;
    }
    ssize_t n;
    char buf[MAX_LINE];
    struct sockaddr_in client_addr;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        int ready_number = poll(event_set, QUEUE_SIZE, -1);
        if (ready_number <= 0) {
            perror("poll failed.");
        }
        // 监听连接完成的事件
        if (event_set[0].revents & POLLRDNORM) {
            socklen_t client_len = sizeof(client_addr);
            int conn_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
            // 顺序设置一个 fd
            for (i = 1; i < QUEUE_SIZE; ++i) {
                if (event_set[i].fd < 0) {
                    event_set[i].fd = conn_fd;
                    event_set[i].events = POLLIN;
                    break;
                }
            }
            // 有多少个连接完成
            if (--ready_number <= 0) {
                continue;
            }
        }

        for (i = 1; i < QUEUE_SIZE; ++i) {
            int socket_fd;
            if ((socket_fd = event_set[i].fd) < 0) {
                continue;
            }
            if (event_set[i].revents & POLLIN) {
                n = read(socket_fd, buf, MAX_LINE);
                if (n > 0) {
                    printf("recv: %s \n", buf);
                    if (write(socket_fd, buf, n) < 0) {
                        perror("write error.");
                    }
                } else if (n == 0 || errno == ECONNRESET) {
                    close(socket_fd);
                    event_set[i].fd = -1;
                } else {
                    perror("read error.");
                }
            }
            if (--ready_number <= 0) {
                break;
            }
        }
    }
#pragma clang diagnostic pop
    close(listen_fd);
    return 0;
}