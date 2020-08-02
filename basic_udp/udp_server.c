//
// Created by shimo on 2020/7/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_LINE 1024

int main() {
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("created socket failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int rt = bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rt < 0) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));

    char buf[MAX_LINE];
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        socklen_t client_len = sizeof(client_addr);
        memset(buf, 0, sizeof(buf));
        recvfrom(socket_fd, buf, MAX_LINE, 0, (struct sockaddr *) &client_addr, &client_len);
        fputs(buf, stdout);
        sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr *) &client_addr, client_len);
    }
#pragma clang diagnostic pop
    return 0;
}