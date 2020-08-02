#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>

/**
 * create a tcp socket fd
 * @param addr bind addr
 * @param port bind port
 * @param block block or not
 * @param sock_addr socket addr
 * @return socket fd, if < 0, it is err
 */
int make_tcp_fd(char *addr, int port, bool block, struct sockaddr_in *sock_addr) {
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    // set addr reuse
    int on = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (!block) {
        // set no_blocking
        int flags = fcntl(sock_fd, F_GETFL, 0);
        fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK);
    }
    bzero(sock_addr, sizeof(*sock_addr));
    sock_addr->sin_family = AF_INET;
    sock_addr->sin_port = htons(port);
    sock_addr->sin_addr.s_addr = inet_addr(addr);
    return sock_fd;
}

/**
 * create a tcp server
 * @param addr listen addr, eg.127.0.0.1
 * @param port listen port, eg.80
 * @param block block or not
 * @return socket fd; if < 0, it is err.
 */
int make_tcp_server(char *addr, int port, int queue_size, bool block) {
    struct sockaddr_in sock_addr;
    int sock_fd = make_tcp_fd(addr, port, block, &sock_addr);
    int rt = bind(sock_fd, (const struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (rt < 0) {
        perror("bind err.");
        return rt;
    }
    int stat = listen(sock_fd, queue_size);
    if (stat < 0) {
        perror("listen err.");
        return stat;
    }
    return sock_fd;
}

/**
 * create a tcp client
 * @param addr bind addr
 * @param port bind port
 * @param block block or not
 * @return socket fd; if < 0, it is err.
 */
int make_tcp_client(char *addr, int port, bool block) {
    struct sockaddr_in sock_addr;
    int sock_fd = make_tcp_fd(addr, port, block, &sock_addr);
    int rt = connect(sock_fd, (const struct sockaddr *) &sock_addr, sizeof(sock_addr));
    if (rt < 0) {
        perror("connect err.");
        return rt;
    }
    return sock_fd;
}