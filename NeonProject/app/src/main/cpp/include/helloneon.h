//
// Created by dingjikerbo on 2018/3/24.
//

#ifndef NEONPROJECT_HELLONEON_H
#define NEONPROJECT_HELLONEON_H

#include <jni.h>
#include <string>

#include <arm_neon.h>
#include <android/log.h>

#define LOG_TAG "bush"

#define LOGD(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern int getCurrentMillisecond();
extern void show8(int8x8_t d);
extern void show16q(int16x8_t q);

#endif //NEONPROJECT_HELLONEON_H
