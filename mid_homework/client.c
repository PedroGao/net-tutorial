//
// Created by shimo on 2020/7/15.
//

#include "../lib/common.h"

int main(int argc, char **argv)
{
    int port = 1024;
    int sockfd = tcp_client("127.0.0.1", port);

    char recv_line[MAX_LEN], send_line[MAX_LEN];
    int n;

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(sockfd, &allreads);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;)
    {
        readmask = allreads;
        int rc = select(sockfd + 1, &allreads, NULL, NULL, NULL);
        if (rc <= 0)
        {
            error(1, errno, "select failed");
        }

        if (FD_ISSET(sockfd, &readmask))
        {
            n = read(sockfd, recv_line, MAX_LEN);
            if (n < 0)
            {
                error(1, errno, "read error");
            }
            else if (n == 0)
            {
                printf("server closed \n");
                break;
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }
        // 标准输入
        if (FD_ISSET(STDIN_FILENO, &readmask))
        {
            if (fgets(send_line, MAX_LEN, stdin) != NULL)
            {
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n')
                {
                    send_line[i - 1] = 0;
                }

                if (strncmp(send_line, "quit", strlen(send_line)) == 0)
                {
                    if (shutdown(sockfd, 1))
                    {
                        error(1, errno, "shutdown failed");
                    }
                }

                size_t rt = write(sockfd, send_line, strlen(send_line));
                if (rt < 0)
                {
                    error(1, errno, "write failed");
                }
            }
        }
    }
#pragma clang diagnostic pop
    exit(0);
}
