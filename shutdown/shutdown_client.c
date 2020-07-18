#include "../lib/common.h"

#define MAXLINE 4096

int main(int argc, char **argv)
{
    // 设置 socket  套接字
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 地址
    struct sockaddr_in server_addr;
    // write zeroes to a byte string
    // 将字符串全部置 0
    bzero(&server_addr, sizeof(server_addr));
    // ipv4 协议
    server_addr.sin_family = AF_INET;
    // 端口
    server_addr.sin_port = htons(1024);
    // 将字符串地址转化为 sin_addr
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 地址的长度
    socklen_t server_len = sizeof(server_addr);
    // 连接
    int connect_rt = connect(socket_fd, (struct sockaddr *)&server_addr, server_len);
    // 连接失败
    if (connect_rt < 0)
    {
        error(1, errno, "connect failed...");
    }

    char send_line[MAXLINE], recv_line[MAXLINE + 1];
    int n;

    // select 描述集
    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(socket_fd, &allreads);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    for (;;)
    {
        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
        if (rc <= 0)
        {
            error(1, errno, "select failed...");
        }
        if (FD_ISSET(socket_fd, &readmask))
        {
            n = read(socket_fd, recv_line, MAXLINE);
            if (n < 0)
            {
                error(1, errno, "read error");
            }
            else if (n == 0)
            {
                error(1, 0, "server terminated \n");
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }
        if (FD_ISSET(0, &readmask))
        {
            if (fgets(send_line, MAXLINE, stdin) != NULL)
            {
                if (strncmp(send_line, "shutdown", 8) == 0)
                {
                    FD_CLR(0, &allreads);
                    if (shutdown(socket_fd, 1))
                    {
                        error(1, errno, "shutdown failed...");
                    }
                }
                else if (strncmp(send_line, "close", 5) == 0)
                {
                    FD_CLR(0, &allreads);
                    if (close(socket_fd))
                    {
                        error(1, errno, "close failed");
                    }
                    sleep(6);
                    exit(0);
                }
                else
                {
                    int i = strlen(send_line);
                    if (send_line[i - 1] == '\n')
                    {
                        send_line[i - 1] = 0;
                    }

                    printf("now sending %s\n", send_line);
                    size_t rt = write(socket_fd, send_line, strlen(send_line));
                    if (rt < 0)
                    {
                        error(1, errno, "write failed");
                    }
                    printf("send bytes: %zu \n", rt);
                }
            }
        }
    }
#pragma clang diagnostic pop
}