//
// Created by shimo on 2020/7/12.
//

#include "../lib/common.h"

#define MAXLINE 4096

static int count;

static void sig_int() {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

static void sig_pipe(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main(int argc, char **argv) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(1024);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rt1 < 0) {
        error(1, errno, "bind failed...");
    }

    int rt2 = listen(listenfd, 128);
    if (rt2 < 0) {
        error(1, errno, "listen failed...");
    }

    signal(SIGINT, sig_int);
    // SIGPIPE 表示读数据读完了
    // signal(SIGPIPE, SIG_IGN);
    signal(SIGPIPE, sig_pipe);

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    connfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len) < 0;
    if (connfd) {
        error(1, errno, "bind failed...");
    }

    char message[MAXLINE];
    count = 0;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;) {
        int n = read(connfd, message, MAXLINE);
        if (n < 0) {
            error(1, errno, "error read");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }
        message[n] = 0;
        printf("received %d bytes: %s\n", n, message);
        count++;

        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);

        sleep(5);

        int write_nc = send(connfd, send_line, strlen(send_line), 0);
        printf("send bytes: %zu \n", write_nc);
        if (write_nc < 0) {
            error(1, errno, "error write");
        }
    }
#pragma clang diagnostic pop
}

