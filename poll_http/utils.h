//
// Created by shimo on 2020/7/19.
//

#ifndef NET_TUTORIAL_UTILS_H
#define NET_TUTORIAL_UTILS_H

#include "event_loop.h"
#include "pthread.h"

void assertInSameThread(struct event_loop *eventLoop) {
    if (eventLoop->owner_thread_id != pthread_self()) {
        sprintf(stderr, "not in the same thread");
        exit(-1);
    }
}

//1： same thread: 0： not the same thread
int isInSameThread(struct event_loop *eventLoop) {
    return eventLoop->owner_thread_id == pthread_self();
}

#endif //NET_TUTORIAL_UTILS_H
