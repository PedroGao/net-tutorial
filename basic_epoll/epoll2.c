#include <unistd.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include "../lib/common.h"

#define MAXEVENTS 128

char rot13_char(char c)
{
  if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
    return c + 13;
  else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
    return c - 13;
  else
    return c;
}

int main(int argc, char **argv)
{
  int listen_fd, socket_fd;
  int n, i;
  int efd;
  struct epoll_event event;
  struct epoll_event *events;

  listen_fd = make_tcp_server("127.0.0.1", 1024, 10, true);

  efd = epoll_create1(0);
  if (efd == -1)
  {
    perror("epoll create failed");
  }

  event.data.fd = listen_fd;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &event) == -1)
  {
    perror("epoll_ctl add listen fd failed");
  }

  /* Buffer where events are returned */
  events = calloc(MAXEVENTS, sizeof(event));

  while (1)
  {
    n = epoll_wait(efd, events, MAXEVENTS, -1);
    printf("epoll_wait wakeup\n");
    for (i = 0; i < n; i++)
    {
      // 判断是否发生错误、或者其他故障之类的，关闭套接字
      if ((events[i].events & EPOLLERR) ||
          (events[i].events & EPOLLHUP) ||
          (!(events[i].events & EPOLLIN)))
      {
        fprintf(stderr, "epoll error\n");
        close(events[i].data.fd);
        continue;
      }
      else if (listen_fd == events[i].data.fd) // 新连接
      {
        struct sockaddr_storage ss;
        socklen_t slen = sizeof(ss);
        int fd = accept(listen_fd, (struct sockaddr *)&ss, &slen);
        if (fd < 0)
        {
          perror("accept failed");
        }
        else
        {
          event.data.fd = fd;
          event.events = EPOLLIN | EPOLLET; // edge-triggered 边缘触发，高性能
          if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1)
          {
            perror("epoll_ctl add connection fd failed");
          }
        }
        continue;
      }
      else // 读数据
      {
        socket_fd = events[i].data.fd;
        printf("get event on socket fd == %d \n", socket_fd);
        while (1)
        {
          char buf[512];
          if ((n = read(socket_fd, buf, sizeof(buf))) < 0)
          {
            if (errno != EAGAIN)
            {
              perror("read error");
              close(socket_fd);
            }
            break;
          }
          else if (n == 0)
          {
            close(socket_fd);
            break;
          }
          else
          {
            for (i = 0; i < n; ++i)
            {
              buf[i] = rot13_char(buf[i]);
            }
            if (write(socket_fd, buf, n) < 0)
            {
              perror("write error");
            }
          }
        }
      }
    }
  }

  free(events);
  close(listen_fd);
}