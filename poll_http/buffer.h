//
// Created by shimo on 2020/7/19.
//

#ifndef NET_TUTORIAL_BUFFER_H
#define NET_TUTORIAL_BUFFER_H

#define INIT_BUFFER_SIZE 65536

struct buffer {
    char *data; //  实际缓冲数据
    int readIndex; // 缓冲读取位置
    int writeIndex; // 缓冲写入位置
    int total_size; // 总大小
};

struct buffer *buffer_new();

void buffer_free(struct buffer *buffer);

int buffer_writeable_size(struct buffer *buffer);

int buffer_readable_size(struct buffer *buffer);

int buffer_front_spare_size(struct buffer *buffer);

// push data to buffer
int buffer_append(struct buffer *buffer, void *data, int size);

int buffer_append_char(struct buffer *buffer, char data);

int buffer_append_string(struct buffer *buffer, char *data);

int buffer_socket_read(struct buffer *buffer, int fd);

char buffer_read_char(struct buffer *buffer);

// 查询 buffer 数据
char *buffer_find_CRLF(struct buffer *buffer);

#endif //NET_TUTORIAL_BUFFER_H
