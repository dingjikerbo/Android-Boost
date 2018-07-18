//
// Created by dingjikerbo on 2018/3/24.
//

#ifndef NEONPROJECT_HELLONEON_H
#define NEONPROJECT_HELLONEON_H

#include <jni.h>
#include <stdlib.h>

#include <arm_neon.h>
#include <android/log.h>

#define LOG_TAG "bush"

#define LOGD(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern int getCurrentMillisecond();
extern int getCurrentMicrosecond();
extern void show8(int8x8_t d);
extern void show8q(int8x16_t q);
extern void show16(int16x4_t q);
extern void show16q(int16x8_t q);
extern void show32q(int32x4_t q);
extern void show32(int32x2_t d);
extern void showu32q(uint32x4_t q);
extern void show32fq(float32x4_t qf);

void sqrtfv_neon(float *x, int n, float *r);

uint16_t fxpt_atan2(const int16_t y, const int16_t x);

#endif //NEONPROJECT_HELLONEON_H
