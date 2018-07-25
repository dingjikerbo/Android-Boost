//
// Created by 李文田 on 2018/7/18.
//
#include "main.h"

float sqrt1(float a, float b) {
    a = fabs(a);
    b = fabs(b);

    if (a > b) {
        return sqrt1(b, a);
    }

    return 0.41421356f * a + b;
}

float sqrt2(float a, float b) {
    return b + 0.428 * a * a / b;
}

float32x4_t sqrt3(float32x4_t qa, float32x4_t qb) {
    qa = vmulq_f32(qa, qa);
    qb = vmlaq_f32(qa, qb, qb);
    return sqrtv(qb);
}

void test_sqrt() {
    int nfft = 1024;

    srand(time(NULL));

    float *fy = malloc(nfft * sizeof(float));
    float *fx = malloc(nfft * sizeof(float));

    for (int i = 0; i < nfft; i++) {
        fy[i] = fabsf(128.0f / (rand() % 256 - 128 + 1e-4));
        fx[i] = fabsf(128.0f / (rand() % 256 - 128 + 1e-4));
    }

    float *f0 = malloc(nfft * sizeof(float));
    float *f1 = malloc(nfft * sizeof(float));
    float *f2 = malloc(nfft * sizeof(float));
    float *f3 = malloc(nfft * sizeof(float));

    int time1 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f0[i] = sqrtf(fy[i] * fy[i] + fx[i] * fx[i]);
    }

    int time2 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f1[i] = sqrt1(fy[i], fx[i]);
    }

    int time3 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f2[i] = sqrt2(fy[i], fx[i]);
    }

    int time4 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i+=4) {
        float32x4_t qa = vld1q_f32(fy + i);
        float32x4_t qb = vld1q_f32(fx + i);
        vst1q_f32(f3 + i, sqrt3(qa, qb));
    }
    int time5 = getCurrentMicrosecond();

    float sum1=0.0f, sum2=0.0f, sum3=0.0f;

    for (int i = 0; i < nfft; i++) {
        sum1 += fabsf(100 * (f1[i] - f0[i]) / f0[i]);
        sum2 += fabsf(100 * (f2[i] - f0[i]) / f0[i]);
        sum3 += fabsf(100 * (f3[i] - f0[i]) / f0[i]);
    }

    LOGD("sqrt1: %.2f%%, sqrt2: %.2f%%, sqrt3: %.2f%%", sum1/nfft, sum2/nfft, sum3/nfft);
    LOGD("sqrt0: %dus, sqrt1: %dus, sqrt2: %dus, sqrt3: %dus", time2-time1, time3-time2, time4-time3, time5-time4);
}