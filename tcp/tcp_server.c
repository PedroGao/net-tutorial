#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "../lib/common.h"

void read_data(int sockfd) {
    ssize_t n;
    char buf[1024];
    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, buf, 1024)) == 0)
            return;
        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(1000);
    }
}

int main(int args, char **argv) {
    int listen_fd, port = 1024, connfd;
    // ipv4
    struct sockaddr_in servaddr;
    // bzero(&servaddr, sizeof(servaddr));
    /* 绑定到port和ip. */
    servaddr.sin_family = AF_INET; /* IPV4 */
    servaddr.sin_port = htons(port); /* 指定端口 */
    // name.sin_addr.s_addr = htonl(INADDR_ANY); /* 通配地址 */
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* 创建字节流类型的IPV4 socket. */
    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    /* 把IPV4地址转换成通用地址格式，同时传递⻓度 */
    if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(listen_fd, 128);
    struct sockaddr_in client;
    socklen_t clilen;

    printf("listening at localhost:%d\n", port);
    // 循环处理请求
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;) {
        clilen = sizeof(client);
        connfd = accept(listen_fd, (struct sockaddr *) &client, &clilen);
        read_data(connfd);
        close(connfd);
    }
#pragma clang diagnostic pop
}
