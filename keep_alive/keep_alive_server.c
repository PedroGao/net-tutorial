#include "common.h"
#include "message_type.h"

static int count;

int main(int argc, char **argv) {
    int sleeping_time = atoi("60");

    int listen_fd;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1024);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int rt1 = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rt1 < 0) {
        error(1, errno, "bind failed ");
    }

    int rt2 = listen(listen_fd, 128);
    if (rt2 < 0) {
        error(1, errno, "listen failed ");
    }

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    connfd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len);
    if (connfd < 0) {
        error(1, errno, "bind failed ");
    }

    message_object message;
    count = 0;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;) {
        int n = read(connfd, (char *) &message, sizeof(message));
        if (n < 0) {
            error(1, errno, "error read");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }

        printf("received %d bytes\n", n);
        count++;

        switch (ntohl(message.type)) {
            case MSG_TYPE1:
                printf("process  MSG_TYPE1 \n");
                break;
            case MSG_TYPE2:
                printf("process  MSG_TYPE2 \n");
                break;
            case MSG_PING: {
                message_object pong_message;
                pong_message.type = MSG_PONG;
                sleep(sleeping_time);
                ssize_t rc = send(connfd, (char *) &pong_message, sizeof(pong_message), 0);
                if (rc < 0) {
                    error(1, errno, "send failure");
                }
                break;
            }
            default:
                error(1, 0, "unknown type (%d)\n", htonl(message.type));
        }
    }
#pragma clang diagnostic pop
}