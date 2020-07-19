//
// Created by shimo on 2020/7/19.
//

#ifndef NET_TUTORIAL_ACCEPTOR_H
#define NET_TUTORIAL_ACCEPTOR_H

#include "../lib/common.h"

#define LISTENQ 128

struct acceptor {
    int listen_port;
    int listen_fd;
};

struct acceptor *acceptor_init(int port);

#endif //NET_TUTORIAL_ACCEPTOR_H
