//
// Created by shimo on 2020/7/19.
//

#ifndef NET_TUTORIAL_EVENT_DISPATCHER_H
#define NET_TUTORIAL_EVENT_DISPATCHER_H

#include "channel.h"

// 抽象 event_dispatcher，对应的实现如 select，poll，epoll
struct event_dispatcher {
    // 实现名称
    const char *name;

    void *(*init)(struct event_loop *eventLoop);

    int (*add)(struct event_loop *eventLoop, struct channel *channel);

    int (*del)(struct event_loop *eventLoop, struct channel *channel);

    int (*update)(struct event_loop *eventLoop, struct channel *channel);

    int (*dispatch)(struct event_loop *eventLoop, struct timeval *);

    void (*clear)(struct event_loop *eventLoop);
};

#endif //NET_TUTORIAL_EVENT_DISPATCHER_H
