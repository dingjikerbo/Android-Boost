//
// Created by 李文田 on 2018/3/21.
//
#include "main.h"
#include <time.h>

int getCurrentMillisecond() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int getCurrentMicrosecond() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000000 + t.tv_nsec / 1000;
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

void show8q(int8x16_t q) {
    LOGD("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", vgetq_lane_s8(q, 0),
         vgetq_lane_s8(q, 1),
         vgetq_lane_s8(q, 2),
         vgetq_lane_s8(q, 3),
         vgetq_lane_s8(q, 4),
         vgetq_lane_s8(q, 5),
         vgetq_lane_s8(q, 6),
         vgetq_lane_s8(q, 7),
		 vgetq_lane_s8(q, 8),
		 vgetq_lane_s8(q, 9),
		 vgetq_lane_s8(q, 10),
		 vgetq_lane_s8(q, 11),
		 vgetq_lane_s8(q, 12),
		 vgetq_lane_s8(q, 13),
		 vgetq_lane_s8(q, 14),
		 vgetq_lane_s8(q, 15));
}

void show16(int16x4_t d) {
    LOGD("%d %d %d %d", vget_lane_s16(d, 0),
         vget_lane_s16(d, 1),
         vget_lane_s16(d, 2),
         vget_lane_s16(d, 3));
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

void show32q(int32x4_t q) {
	LOGD("%d %d %d %d", vgetq_lane_s32(q, 0),
		 vgetq_lane_s32(q, 1),
		 vgetq_lane_s32(q, 2),
		 vgetq_lane_s32(q, 3));
}

void showu32q(uint32x4_t q) {
	LOGD("%d %d %d %d", vgetq_lane_u32(q, 0),
		 vgetq_lane_u32(q, 1),
		 vgetq_lane_u32(q, 2),
		 vgetq_lane_u32(q, 3));
}

void show32(int32x2_t d) {
	LOGD("%d %d", vget_lane_s32(d, 0), vget_lane_s32(d, 1));
}

void show32fq(float32x4_t qf) {
	LOGD("%f %f %f %f", vgetq_lane_f32(qf, 0), vgetq_lane_f32(qf, 1),
		 vgetq_lane_f32(qf, 2), vgetq_lane_f32(qf, 3));
}