#ifndef NET_TUTORIAL_MESSAGE_TYPE_H
#define NET_TUTORIAL_MESSAGE_TYPE_H

#include <ntsid.h>

typedef struct {
    u_int32_t type;
    char data[1024];
} message_object;

#define MSG_PING 1
#define MSG_PONG 2
#define MSG_TYPE1 11
#define MSG_TYPE2 21

#endif