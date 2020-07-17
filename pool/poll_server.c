#include "../lib/common.h"

#define INIT_SIZE 2

int main(int argc, char **argv) {
    int listen_fd, connected_fd;
    int ready_number;
    ssize_t n;
    char buf[MAX_LEN];
    struct sockaddr_in client_addr;

    listen_fd = tcp_server_listen(1024);

    // 初始化 pollfd 数组，第一个元素是 listen_fd，其余的用来记录将要连接的 connect
    struct pollfd event_set[INIT_SIZE];
    event_set[0].fd = listen_fd;
    event_set[0].events = POLLRDNORM;

    // 用 -1 表示这个数组位置还没被占用
    int i;
    for (i = 1; i < INIT_SIZE; i++) {
        event_set[i].fd = -1;
    }
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;) {
        int ready_number = poll(event_set, INIT_SIZE, -1);
        if (ready_number < 0) {
            error(1, errno, "pool failed");
        }

        if (event_set[0].revents & POLLRDNORM) {
            socklen_t client_len = sizeof(client_addr);
            connected_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);

            // find position to record fd
            for (i = 0; i < INIT_SIZE; ++i) {
                if (event_set[i].fd < 0) {
                    event_set[i].fd = connected_fd;
                    event_set[i].events = POLLRDNORM;
                    break;
                }
            }

            if (i == INIT_SIZE) {
                error(1, errno, "can not hold so many clients");
            }

            if (--ready_number <= 0) {
                continue;
            }
        }

        for (i = 0; i < INIT_SIZE; ++i) {
            int socket_fd;
            if ((socket_fd = event_set[i].fd) < 0) {
                continue;
            }
            if (event_set[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(socket_fd, buf, MAX_LEN)) > 0) {
                    if (write(socket_fd, buf, n) < 0) {
                        error(1, errno, "write error");
                    }
                } else if (n == 0 || errno == ECONNRESET) {
                    close(socket_fd);
                    event_set[i].fd = -1;
                } else {
                    error(1, errno, "read error");
                }
                if (--ready_number <= 0) {
                    break;
                }
            }
        }
    }
#pragma clang diagnostic pop
}