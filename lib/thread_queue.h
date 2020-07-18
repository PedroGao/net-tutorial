#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
  int number;            // 队列中句柄的最大个数
  int *fd;               // 数组指针
  int front;             // 头位置
  int rear;              // 队尾
  pthread_mutex_t mutex; // 锁
  pthread_cond_t cond;   // 条件变量
} block_queue;

// 初始化队列
void block_queue_init(block_queue *blockQueue, int number)
{
  blockQueue->number = number;
  blockQueue->fd = (int *)calloc(number, sizeof(int));
  blockQueue->front = blockQueue->rear = 0;
  pthread_mutex_init(&blockQueue->mutex, NULL);
  pthread_cond_init(&blockQueue->cond, NULL);
}

// 向对列中 PUSH 一个 fd
void block_queue_push(block_queue *blockQueue, int fd)
{
  // 先锁住
  pthread_mutex_lock(&blockQueue->mutex);
  // 将 fd 放在队尾
  blockQueue->fd[blockQueue->rear] = fd;
  // 已经在最后
  if (++blockQueue->rear == blockQueue->number)
  {
    blockQueue->rear = 0;
  }
  printf("push fd %d", fd);
  // 通知其他线程，有 fd 进来
  pthread_cond_signal(&blockQueue->cond);
  // unlock
  pthread_mutex_unlock(&blockQueue->mutex);
}

// pop
int block_queue_pop(block_queue *blockQueue)
{
  // 加锁
  pthread_mutex_lock(&blockQueue->mutex);
  //判断队列里没有新的连接字可以处理，就一直条件等待，直到有新的连接字入队列
  while (blockQueue->front == blockQueue->rear)
  {
    pthread_cond_wait(&blockQueue->cond, &blockQueue->mutex);
  }
  int fd = blockQueue->fd[blockQueue->front];
  if (++blockQueue->front == blockQueue->number)
  {
    blockQueue->front = 0;
  }
  printf("pop fd %d", fd);
  pthread_mutex_unlock(&blockQueue->mutex);
  return fd;
}