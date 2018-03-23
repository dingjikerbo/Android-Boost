//
// Created by 李文田 on 2018/3/21.
//
#include <time.h>

int getCurrentMillisecond() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return static_cast<int>(static_cast<uint64_t>(t.tv_sec) * 1000 +
                            static_cast<uint64_t>(t.tv_nsec) / 1000000);
}