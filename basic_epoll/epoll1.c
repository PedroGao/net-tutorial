#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include "../lib/common.h"

#define MAX_EVENTS 10
#define MAX_LINE 1024

int main(int args, char **argv)
{
  int sock_fd = make_tcp_client("127.0.0.1", 1024, true);
  if (sock_fd < 0)
  {
    perror("make tcp client err.");
    exit(EXIT_FAILURE);
  }
  int efd = epoll_create1(0);
  if (efd < 0)
  {
    perror("epoll create err.");
    exit(EXIT_FAILURE);
  }

  struct epoll_event event1;
  struct epoll_event event2;
  struct epoll_event *events;
  event1.data.fd = STDIN_FILENO;
  event1.events = EPOLLIN;

  if (epoll_ctl(efd, EPOLL_CTL_ADD, STDIN_FILENO, &event1) == -1)
  {
    perror("epoll_ctl add stdin fd failed");
  }
  event2.data.fd = sock_fd;
  event2.events = EPOLLIN;
  if (epoll_ctl(efd, EPOLL_CTL_ADD, sock_fd, &event2) == -1)
  {
    perror("epoll_ctl add listen fd failed");
  }
  ssize_t n;
  char recv_line[MAX_LINE], send_line[MAX_LINE];
  // events = calloc(MAX_EVENTS, sizeof(event1));
  while (1)
  {
    n = epoll_wait(efd, events, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++)
    {
      if (events[i].data.fd == STDIN_FILENO && (events[i].events & EPOLLIN))
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
      if (events[i].data.fd == sock_fd && (events[1].events & EPOLLIN))
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
    }
  }
  close(efd);
  close(sock_fd);
  return 0;
}