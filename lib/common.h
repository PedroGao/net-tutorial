//
// Created by shimo on 2020/7/9.
//

#ifndef NET_TUTORIAL_COMMON_H
#define NET_TUTORIAL_COMMON_H

// #include <zconf.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <poll.h>
#include <syslog.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/select.h>
#include <unistd.h>

#define MAX_LEN 1024

/* 从socketfd描述字中读取"size"个字节. */
size_t readn(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int length = size;

    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            if (errno == EINTR)
                continue;     /* 考虑非阻塞的情况，这里需要再次调用read */
            else
                return (-1);
        } else if (result == 0)
            break;                /* EOF(End of File)表示套接字关闭 */
        length -= result;
        buffer_pointer += result;
    }
    return (size - length);        /* 返回的是实际读取的字节数*/
}

/* error - print a diagnostic and optionally exit */
void error(int status, int err, char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
        exit(status);
}

int tcp_client(char *addr, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    inet_pton(AF_INET, addr, &client_addr.sin_addr);
    socklen_t client_len = sizeof(client_addr);
    int connect_rt = connect(sockfd, (struct sockaddr *) &client_addr, client_len);
    if (connect_rt < 0) {
        error(1, errno, "connect fail");
    }
    return sockfd;
}

int tcp_server_listen(int port) {
    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    int on = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    int rt1 = bind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (rt1 < 0) {
        error(1, errno, "bind failed ");
    }
    int rt2 = listen(listen_fd, 128);
    if (rt2 < 0) {
        error(1, errno, "listen failed ");
    }
    return listen_fd;
}

#endif //NET_TUTORIAL_COMMON_H
