#include "../lib/common.h"
#include "../lib/thread_queue.h"

#define MAX_LINE 4096
#define BLOCK_QUEUE_SIZE 10
#define THREAD_NUMBER 10

typedef struct
{
  pthread_t thread_tid; /* thread ID */
  long thread_count;    /* # connections handled */
} Thread;

Thread *thread_array;

char rot13_char(char c)
{
  if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
    return c + 13;
  else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
    return c - 13;
  else
    return c;
}

void loop_echo(int fd)
{
  char outbuf[MAX_LINE + 1];
  size_t outbuf_used = 0;
  ssize_t result;
  while (1)
  {
    char ch;
    result = recv(fd, &ch, 1, 0);

    //断开连接或者出错
    if (result == 0)
    {
      break;
    }
    else if (result == -1)
    {
      error(1, errno, "read error");
      break;
    }

    if (outbuf_used < sizeof(outbuf))
    {
      outbuf[outbuf_used++] = rot13_char(ch);
    }

    if (ch == '\n')
    {
      send(fd, outbuf, outbuf_used, 0);
      outbuf_used = 0;
      continue;
    }
  }
}

void thread_run(void *arg)
{
  pthread_t tid = pthread_self();
  pthread_detach(tid);
  block_queue *blockQueue = (block_queue *)arg;
  while (1)
  {
    int fd = block_queue_pop(blockQueue);
    printf("get fd in thread, fd==%d, tid == %d", fd, tid);
    loop_echo(fd);
  }
}

int main(int argc, char const *argv[])
{
  int listener_fd = tcp_server_listen(1024);
  block_queue blockQueue;
  block_queue_init(&blockQueue, BLOCK_QUEUE_SIZE);
  thread_array = calloc(THREAD_NUMBER, sizeof(Thread));
  int i;
  for (i = 0; i < THREAD_NUMBER; i++)
  {
    pthread_create(&(thread_array[i].thread_tid), NULL, &thread_run, (void *)&blockQueue);
  }
  while (1)
  {
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener_fd, (struct sockaddr *)&ss, &slen);
    if (fd < 0)
    {
      error(1, errno, "accept failed");
    }
    else
    {
      block_queue_push(&blockQueue, fd);
    }
  }
}
