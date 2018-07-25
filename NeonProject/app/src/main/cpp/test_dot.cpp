//
// Created by 李文田 on 2018/7/23.
//

#include "main.h"


int dot(int16_t *a, int16_t *b, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

int dot_neon1(int16_t *a, int16_t *b, int n) {
    int k32 = n&(~31);

    int32x4_t qs = vdupq_n_s32(0);

    for (int i = 0; i < k32; i += 32) {
        int16x8x4_t q1 = vld4q_s16(a + i);
        int16x8x4_t q2 = vld4q_s16(b + i);

        int32x4_t qs1 = vmull_s16(vget_low_s16(q1.val[0]), vget_low_s16(q2.val[0]));
        int32x4_t qs2 = vmull_s16(vget_low_s16(q1.val[1]), vget_low_s16(q2.val[1]));
        int32x4_t qs3 = vmull_s16(vget_low_s16(q1.val[2]), vget_low_s16(q2.val[2]));
        int32x4_t qs4 = vmull_s16(vget_low_s16(q1.val[3]), vget_low_s16(q2.val[3]));

        qs1 = vmlal_s16(qs1, vget_high_s16(q1.val[0]), vget_high_s16(q2.val[0]));
        qs2 = vmlal_s16(qs2, vget_high_s16(q1.val[1]), vget_high_s16(q2.val[1]));
        qs3 = vmlal_s16(qs3, vget_high_s16(q1.val[2]), vget_high_s16(q2.val[2]));
        qs4 = vmlal_s16(qs4, vget_high_s16(q1.val[3]), vget_high_s16(q2.val[3]));

        int32x4_t qs12 = vaddq_s32(qs1, qs2);
        int32x4_t qs34 = vaddq_s32(qs3, qs4);

        int32x4_t qs1234 = vaddq_s32(qs12, qs34);

        qs = vaddq_s32(qs, qs1234);
    }

    int32x2_t qt = vpadd_s32(vget_low_s32(qs), vget_high_s32(qs));
    return vget_lane_s32(vpadd_s32(qt, qt), 0) >> 1;
}

int dot_neon2(int16_t *a, int16_t *b, int n) {
    int k7 = n&(~7);

    int32x4_t qs = vdupq_n_s32(0);

    for (int i = 0; i < k7; i += 8) {
        int16x8_t q1 = vld1q_s16(a + i);
        int16x8_t q2 = vld1q_s16(b + i);

        int32x4_t qt1 = vmull_s16(vget_low_s16(q1), vget_low_s16(q2));
        int32x4_t qt2 = vmull_s16(vget_high_s16(q1), vget_high_s16(q2));

        qs = vaddq_s32(qs, vaddq_s32(qt1, qt2));
    }

    int32x2_t qt = vadd_s32(vget_low_s32(qs), vget_high_s32(qs));
    return vget_lane_s32(qt, 0) + vget_lane_s32(qt, 1);
}

int dot_neon3(int16_t *a, int16_t *b, int n) {
    int k32 = n&(~31);

    int32x4_t qs = vdupq_n_s32(0);

    for (int i = 0; i < k32; i += 16) {
        int16x8_t q1 = vld1q_s16(a + i);
        int16x8_t q2 = vld1q_s16(a + i + 8);
        int16x8_t q3 = vld1q_s16(b + i);
        int16x8_t q4 = vld1q_s16(b + i + 8);

        int32x4_t qt1 = vmull_s16(vget_low_s16(q1), vget_low_s16(q2));
        int32x4_t qt2 = vmull_s16(vget_high_s16(q1), vget_high_s16(q2));
        int32x4_t qt3 = vmull_s16(vget_low_s16(q3), vget_low_s16(q3));
        int32x4_t qt4 = vmull_s16(vget_high_s16(q4), vget_high_s16(q4));

        int32x4_t qt12 = vaddq_s32(qt1, qt2);
        int32x4_t qt34 = vaddq_s32(qt3, qt4);

        qs = vaddq_s32(qs, vaddq_s32(qt12, qt34));
    }

    int32x2_t qt = vpadd_s32(vget_low_s32(qs), vget_high_s32(qs));
    return vget_lane_s32(vpadd_s32(qt, qt), 0) >> 1;
}

int dot_neon4(int16_t *a, int16_t *b, int n) {
    int32x4_t qs1 = vmovq_n_s32(0);
    for (int j = 0; j < n; j += 4) {
        int16x4_t dsinc = vld1_s16(a + j);
        int16x4_t diptr = vld1_s16(b + j);
        qs1 = vmlal_s32(qs1, dsinc, diptr);
    }
    int32x2_t ds1 = vadd_s32(vget_low_s32(qs1), vget_high_s32(qs1));
    return vget_lane_s32(vpadd_s32(ds1, ds1), 0);
}

void test_dot() {
    int count = 1024;
    int *number = malloc(sizeof(int) * count);
    int *table = malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++) {
        number[i] = i * 2;
        table[i] = i * 3;
    }

    int kn = 1000, sum0, sum1, sum2, sum3, sum4;
    int time0 = getCurrentMicrosecond();
    for (int i = 0; i < kn; i++) {
        sum0 = dot(number, table, count);
    }

    int time1 = getCurrentMicrosecond();
    for (int i = 0; i < kn; i++) {
        sum1 = dot_neon1(number, table, count);
    }
    int time2 = getCurrentMicrosecond();

    for (int i = 0; i < kn; i++) {
        sum2 = dot_neon2(number, table, count);
    }
    int time3 = getCurrentMicrosecond();

    for (int i = 0; i < kn; i++) {
        sum3 = dot_neon3(number, table, count);
    }
    int time4 = getCurrentMicrosecond();

    for (int i = 0; i < kn; i++) {
        sum4 = dot_neon4(number, table, count);
    }
    int time5 = getCurrentMicrosecond();

    LOGD("sum0 = %d, time1 = %d", sum0, time1-time0);
    LOGD("sum1 = %d, time1 = %d", sum1, time2-time1);
    LOGD("sum2 = %d, time2 = %d", sum2, time3-time2);
    LOGD("sum3 = %d, time3 = %d", sum3, time4-time3);
    LOGD("sum4 = %d, time4 = %d", sum4, time5-time4);

};