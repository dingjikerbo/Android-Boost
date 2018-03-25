//
// Created by 李文田 on 2018/3/21.
//
#include "helloneon.h"
#include <time.h>

int getCurrentMillisecond() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return static_cast<int>(static_cast<uint64_t>(t.tv_sec) * 1000 +
	                        static_cast<uint64_t>(t.tv_nsec) / 1000000);
}

void show8(int8x8_t d) {
	LOGD("%d %d %d %d %d %d %d %d", vget_lane_s8(d, 0),
	     vget_lane_s8(d, 1),
	     vget_lane_s8(d, 2),
	     vget_lane_s8(d, 3),
	     vget_lane_s8(d, 4),
	     vget_lane_s8(d, 5),
	     vget_lane_s8(d, 6),
	     vget_lane_s8(d, 7));
}

void show16q(int16x8_t q) {
	LOGD("%d %d %d %d %d %d %d %d", vgetq_lane_s16(q, 0),
	     vgetq_lane_s16(q, 1),
	     vgetq_lane_s16(q, 2),
	     vgetq_lane_s16(q, 3),
	     vgetq_lane_s16(q, 4),
	     vgetq_lane_s16(q, 5),
	     vgetq_lane_s16(q, 6),
	     vgetq_lane_s16(q, 7));
}