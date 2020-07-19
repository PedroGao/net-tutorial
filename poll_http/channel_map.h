//
// Created by shimo on 2020/7/19.
//

#ifndef NET_TUTORIAL_CHANNEL_MAP_H
#define NET_TUTORIAL_CHANNEL_MAP_H

#include "channel.h"

/**
 * channel 映射表，key 为对应的 socket 描述符
 */

struct channel_map {
    void **entries;

    // number of entries
    int nentries;
};

int map_make_space(struct channel_map *map, int slot, int msize);

void map_init(struct channel_map *map);

void map_clear(struct channel_map *map);

#endif //NET_TUTORIAL_CHANNEL_MAP_H
