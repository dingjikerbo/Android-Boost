//
// Created by dingjikerbo on 2018/3/24.
//

#ifndef NEONPROJECT_HELLONEON_H
#define NEONPROJECT_HELLONEON_H

#include <jni.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <android/log.h>
#include "neonutil.h"
#include "fast_math.h"

#define LOG_TAG "bush"

#define LOGD(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern int getCurrentMillisecond();
extern int getCurrentMicrosecond();

extern void test_sincos();


#endif //NEONPROJECT_HELLONEON_H
