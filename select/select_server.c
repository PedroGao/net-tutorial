#include "../lib/common.h"

//select 检测套接字可写，完全是基于套接字本身的特性来说的，具体来说有以下几种情况。
//第一种是套接字发送缓冲区足够大，如果我们使用套接字进行 write 操作，将不会被阻塞，直接返回。
//第二种是连接的写半边已经关闭，如果继续进行写操作将会产生 SIGPIPE 信号。
//第三种是套接字上有错误待处理，使用 write 函数去执行写操作，不阻塞，且返回 -1。
//总结成一句话就是，内核通知我们套接字可以往里写了，使用 write 函数就不会阻塞。

int main(int argc, char **argv) {
    int socket_fd = tcp_client("127.0.0.1", 1024);
    char recv_line[MAX_LEN], send_line[MAX_LEN];
    int n;

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    // 将标准输入 0 纳入监听
    FD_SET(0, &allreads);
    // 监听 socket
    FD_SET(socket_fd, &allreads);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        // 每次都得重新设置一次 readmask
        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);
        if (rc <= 0) {
            error(1, errno, "select failed");
        }

        if (FD_ISSET(socket_fd, &readmask)) {
            n = read(socket_fd, recv_line, MAX_LEN);
            if (n < 0) {
                error(1, errno, "read error");
            } else if (n == 0) {
                error(1, 0, "server terminated\n");
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &readmask)) {
            if (fgets(send_line, MAX_LEN, stdin) != NULL) {
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n') {
                    send_line[i - 1] = 0;
                }

                printf("now sending %s\n", send_line);
                size_t rt = write(socket_fd, send_line, strlen(send_line));
                if (rt < 0) {
                    error(1, errno, "write failed");
                }
                printf("send bytes: %zu \n", rt);
            }
        }
    }
#pragma clang diagnostic pop
}