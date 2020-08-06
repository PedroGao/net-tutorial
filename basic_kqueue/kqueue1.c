#include <unistd.h>
#include <stdlib.h>
#include <sys/event.h>
#include <sys/types.h>
#include <../lib/common.h>

#define MAX_LINE 1024
#define MAX_EVENT 2

int main()
{
    int sock_fd;
    sock_fd = make_tcp_client("127.0.0.1", 1024, true);
    if (sock_fd < 0)
    {
        perror("make tcp client err.");
        exit(EXIT_FAILURE);
    }
    int kq = kqueue();

    struct kevent evt[MAX_EVENT];                                       // 新建事件
    EV_SET(&evt[0], sock_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0); // 设置事件
    EV_SET(&evt[1], STDIN_FILENO, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

    ssize_t n;
    char recv_line[MAX_LINE], send_line[MAX_LINE];
    // 返回事件
    struct kevent events[MAX_EVENT];
    while (1)
    {
        int ret = kevent(kq, evt, MAX_EVENT, events, MAX_EVENT, NULL);
        if (ret <= 0)
        {
            perror("kqueue failed.");
        }
        for (int i = 0; i < MAX_EVENT; i++)
        {
            int ready_fd = events[i].ident;
            if (ready_fd == STDIN_FILENO)
            {
                if (fgets(send_line, MAX_LINE, stdin) != NULL)
                {
                    printf("now sending %s\n", send_line);
                    n = write(sock_fd, send_line, strlen(send_line));
                    if (n < 0)
                    {
                        perror("write failed.");
                    }
                    printf("send bytes: %zu \n", n);
                }
            }
            else if (ready_fd == sock_fd)
            {
                n = read(sock_fd, recv_line, MAX_LINE);
                if (n < 0)
                {
                    perror("read err.");
                }
                else if (n == 0)
                {
                    perror("server terminated \n");
                }
                fputs(recv_line, stdout);
                fputs("\n", stdout);
            }
            else
            {
                perror("no fd selected");
            }
        }
    }

    close(sock_fd);
    return 0;
}
