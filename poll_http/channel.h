//
// Created by shimo on 2020/7/19.
//

#ifndef NET_TUTORIAL_CHANNEL_H
#define NET_TUTORIAL_CHANNEL_H

#include "common.h"
#include "buffer.h"

// timeout
#define EVENT_TIMEOUT 0x01

/** Wait for a socket or FD to become readable */
#define EVENT_READ 0x02

/** Wait for a socket or FD to become writeable */
#define EVENT_WRITE 0x04

/** Wait for a POSIX signal to be raised*/
#define EVENT_SIGNAL 0x08


typedef int (*event_read_callback)(void *data);

typedef int (*event_write_callback)(void *data);


struct channel {
    int fd;
    int events; // 表示 event 类型

    event_read_callback eventReadCallback;
    event_write_callback eventWriteCallback;
    void *data; // callback data, maybe event_loop or tcp_server, tcp_connection
};

struct channel *channel_new(int fd, int events, event_read_callback eventReadCallback,
                            event_write_callback eventWriteCallback, void *data);

int channel_write_event_is_enabled(struct channel *channel);

int channel_write_event_enable(struct channel *channel);

int channel_write_event_disable(struct channel *channel);

#endif //NET_TUTORIAL_CHANNEL_H
