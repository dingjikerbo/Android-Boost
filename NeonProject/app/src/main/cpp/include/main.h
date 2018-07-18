//
// Created by dingjikerbo on 2018/3/24.
//

#ifndef NEONPROJECT_HELLONEON_H
#define NEONPROJECT_HELLONEON_H

#include <jni.h>
#include <stdlib.h>
#include <time.h>

#include <arm_neon.h>
#include <android/log.h>

#include <math.h>

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
void test_sqrt();

static inline float32x4_t invsqrtv(float32x4_t x) {
    float32x4_t sqrt_reciprocal = vrsqrteq_f32(x);
    return vrsqrtsq_f32(x * sqrt_reciprocal, sqrt_reciprocal) * sqrt_reciprocal;
}

// approximative quadword float square root
static inline float32x4_t sqrtv(float32x4_t x) {
    float32x4_t q_inv = x * invsqrtv(x);

    float32x4_t q_zero = vmovq_n_f32(0.00000001f);
    float32x4_t q_abs = vabsq_f32(q_inv);
    uint32x4_t q_temp = vcgtq_f32(q_abs, q_zero);
    q_inv = vandq_s8(q_inv, q_temp);
    return q_inv;
}

static inline float fast_atan2_2_boost2(float y, float x) {
    float a = fminf(fabsf(x), fabsf(y)) / fmaxf(fabsf(x), fabsf(y));
    float s = a * a;
//    float r = ((0.15931422f * s - 0.327622764f) * s + 1.0f) * a;

    float r = (0.15931422f * s * s + 1.0f - 0.327622764f * s) * a;

    if (fabsf(y) > fabsf(x)) r = (float) M_PI_2 - r;
    if (x < 0) r = (float) M_PI - r;
    if (y < 0) r = -r;

    return r;
}

static inline float32x4_t fast_atan2f_neon(float32x4_t qy, float32x4_t qx) {
    float32x4_t qzero = vdupq_n_f32(0.0f);

    float32x4_t qyneg = vcleq_f32(qy, qzero);
    float32x4_t qxneg = vcleq_f32(qx, qzero);

    float32x4_t qabsy = vbslq_f32(qyneg, vmulq_n_f32(qy, -1.0f), qy);
    float32x4_t qabsx = vbslq_f32(qxneg, vmulq_n_f32(qx, -1.0f), qx);

    float32x4_t qrel = vcleq_f32(qabsx, qabsy);

    float32x4_t qmin = vbslq_f32(qrel, qabsx, qabsy);
    float32x4_t qmax = vbslq_f32(qrel, qabsy, qabsx);

    float32x4_t qa = vmulq_f32(qmin, vrecpeq_f32(qmax));
    float32x4_t qs = vmulq_f32(qa, qa);

    float32x4_t qr = vmlaq_f32(vdupq_n_f32(-0.327622764f), qr, qs);
    qr = vmlaq_f32(vdupq_n_f32(1.0f), qr, qs);
    qr = vmulq_f32(qr, qa);

    qr = vbslq_f32(qrel, vsubq_f32(vdupq_n_f32(M_PI_2), qr), qr);
    qr = vbslq_f32(qxneg, vsubq_f32(vdupq_n_f32(M_PI), qr), qr);
    qr = vbslq_f32(qyneg, vmulq_n_f32(qr, -1.0f), qr);

    return qr;
}

void test_atan2();

#endif //NEONPROJECT_HELLONEON_H
