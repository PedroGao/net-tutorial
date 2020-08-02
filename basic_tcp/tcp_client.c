//
// Created by shimo on 2020/7/21.
//
#include <unistd.h>
#include <stdlib.h>

#include "../lib/common.h"

#define MESSAGE_SIZE 100

int main(int argc, char **argv) {
    int socket_fd;
    socket_fd = make_tcp_client("127.0.0.1", 1024, true);
    if (socket_fd < 0) {
        printf("make tcp client error\n");
        exit(EXIT_FAILURE);
    }
    char send_buf[MESSAGE_SIZE];
    char recv_buf[MESSAGE_SIZE];
    while (fgets(send_buf, sizeof(send_buf), stdin) != NULL) {
        write(socket_fd, send_buf, strlen(send_buf));
        ssize_t n = read(socket_fd, recv_buf, sizeof(recv_buf));
        if (n > 0) {
            fputs(recv_buf, stdout);
        }
        memset(send_buf, 0, sizeof(send_buf));
        memset(recv_buf, 0, sizeof(recv_buf));
    }
    close(socket_fd);
}
