//
// Created by dingjikerbo on 2018/4/1.
//

#include <math.h>
#include <cmath>
#include "main.h"

typedef uint8_t uchar;

void
test_neon(const uchar *S_, int dwidth, uchar *D_, uint16_t *x_ofs) {
    int *S = (int *) S_, *D = (int *) D_;

    for (int x = 0; x < dwidth; x += 4) {
        uint16x4_t qofs = vld1_u16(x_ofs + x);

        uint ofs0 = vget_lane_u16(qofs, 0);
        uint ofs1 = vget_lane_u16(qofs, 1);
        uint ofs2 = vget_lane_u16(qofs, 2);
        uint ofs3 = vget_lane_u16(qofs, 3);

        int32x2_t q1 = vld1_s32(S + ofs0);
        int32x2_t q2 = vld1_s32(S + ofs1);
        int32x2_t q3 = vld1_s32(S + ofs2);
        int32x2_t q4 = vld1_s32(S + ofs3);

        int32x2_t s0 = vext_s32(vrev64_s32(q1), q2, 1);
        int32x2_t s1 = vext_s32(vrev64_s32(q3), q4, 1);

        int32x4_t qs = vcombine_s32(s0, s1);

        vst1q_s32(D + x, qs);
    }
}

void
test_neon1(const uchar *S_, int dwidth, uchar *D_, uint16_t *x_ofs) {
    int *S = (int *) S_, *D = (int *) D_;

    for (int x = 0; x < dwidth; x += 4) {
        uint16x4_t qofs = vld1_u16(x_ofs + x);

        uint ofs0 = vget_lane_u16(qofs, 0);
        uint ofs1 = vget_lane_u16(qofs, 1);
        uint ofs2 = vget_lane_u16(qofs, 2);
        uint ofs3 = vget_lane_u16(qofs, 3);

        int32x2_t q1 = vld1_s32(S + ofs0);
        int32x2_t q3 = vld1_s32(S + ofs2);
        int32x2_t q2 = vld1_s32(S + ofs1);
        int32x2_t q4 = vld1_s32(S + ofs3);

        int32x4_t qq0 = vcombine_s32(q1, q3);
        int32x4_t qq1 = vcombine_s32(q2, q4);
        int32x4_t qs = vtrnq_s32(qq0, qq1).val[0];

        vst1q_s32(D + x, qs);
    }
}

void
test_neon2(const uchar *S_, int dwidth, uchar *D_, uint16_t *x_ofs) {
    int *S = (int *) S_, *D = (int *) D_;

    for (int x = 0; x < dwidth; x += 8) {
        uint16x8_t qofs = vld1q_u16(x_ofs + x);

        uint ofs0 = vgetq_lane_u16(qofs, 0);
        uint ofs2 = vgetq_lane_u16(qofs, 2);
        uint ofs1 = vgetq_lane_u16(qofs, 1);
        uint ofs3 = vgetq_lane_u16(qofs, 3);
        uint ofs4 = vgetq_lane_u16(qofs, 4);
        uint ofs6 = vgetq_lane_u16(qofs, 6);
        uint ofs5 = vgetq_lane_u16(qofs, 5);
        uint ofs7 = vgetq_lane_u16(qofs, 7);

        int32x2_t q0 = vld1_s32(S + ofs0);
        int32x2_t q2 = vld1_s32(S + ofs2);
        int32x2_t q1 = vld1_s32(S + ofs1);
        int32x2_t q3 = vld1_s32(S + ofs3);
        int32x2_t q4 = vld1_s32(S + ofs4);
        int32x2_t q6 = vld1_s32(S + ofs6);
        int32x2_t q5 = vld1_s32(S + ofs5);
        int32x2_t q7 = vld1_s32(S + ofs7);

        int32x4_t qq0 = vcombine_s32(q0, q2);
        int32x4_t qq1 = vcombine_s32(q1, q3);
        int32x4_t qs1 = vtrnq_s32(qq0, qq1).val[0];

        int32x4_t qq2 = vcombine_s32(q4, q6);
        int32x4_t qq3 = vcombine_s32(q5, q7);
        int32x4_t qs2 = vtrnq_s32(qq2, qq3).val[0];

        vst1q_s32(D + x, qs1);
        vst1q_s32(D + x + 4, qs2);
    }
}

void
test_neon3(const uchar *S_, int dwidth, uchar *D_, uint16_t *x_ofs) {
    int *S = (int *) S_, *D = (int *) D_;

    for (int x = 0; x < dwidth; x += 16) {
        uint16x8_t qofs = vld1q_u16(x_ofs + x);

        uint ofs0 = vgetq_lane_u16(qofs, 0);
        uint ofs2 = vgetq_lane_u16(qofs, 2);
        uint ofs1 = vgetq_lane_u16(qofs, 1);
        uint ofs3 = vgetq_lane_u16(qofs, 3);
        uint ofs4 = vgetq_lane_u16(qofs, 4);
        uint ofs6 = vgetq_lane_u16(qofs, 6);
        uint ofs5 = vgetq_lane_u16(qofs, 5);
        uint ofs7 = vgetq_lane_u16(qofs, 7);

        int32x2_t q0 = vld1_s32(S + ofs0);
        int32x2_t q2 = vld1_s32(S + ofs2);
        int32x2_t q1 = vld1_s32(S + ofs1);
        int32x2_t q3 = vld1_s32(S + ofs3);

        int32x2_t q4 = vld1_s32(S + ofs4);
        int32x2_t q6 = vld1_s32(S + ofs6);
        int32x2_t q5 = vld1_s32(S + ofs5);
        int32x2_t q7 = vld1_s32(S + ofs7);

        int32x4_t qq0 = vcombine_s32(q0, q2);
        int32x4_t qq1 = vcombine_s32(q1, q3);
        int32x4_t qs1 = vtrnq_s32(qq0, qq1).val[0];

        int32x4_t qq2 = vcombine_s32(q4, q6);
        int32x4_t qq3 = vcombine_s32(q5, q7);
        int32x4_t qs2 = vtrnq_s32(qq2, qq3).val[0];

        vst1q_s32(D + x, qs1);
        vst1q_s32(D + x + 4, qs2);
    }
}

void
test_c(const uchar *S, int dwidth, uchar *D, uint16_t *x_ofs) {
    for(int x = 0; x < dwidth; x++ ) {
        *(int *) (D + x * 4) = *(int *) (S + x_ofs[x] * 4);
    }
}

//static void test() {
//    int8_t a[] = {
//            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
//    };
//    int8_t b[] = {
//            0, 9, 1, 2, 0, 1, 1, 0
//    };
//    int8x8_t d1 = vld1_s8(a);
//    int8x8_t d2 = vld1_s8(b);
//    int8x8_t d3 = vtr(d1, d2);
//    show8(d3);
//}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    int width = 40000;
    float scale = 0.5f;
    uint16_t *x_ofs = (uint16_t *) malloc(width * sizeof(int));
    for (int i = 0; i < width; i++) {
        x_ofs[i] = std::floor(scale * i);
    }

    for (int i = 0; i < width; i++) {
//        LOGD("xofs[%d] = %d", i, x_ofs[i]);
    }

    srand(time(nullptr));

    uchar *S = (uchar *) malloc(width * scale * 4);
    for (int i = 0; i < width * scale; i++) {
//        S[i] = rand() % 256;
        S[i] = i / 4;
    }

    uchar *D = (uchar *) malloc(width * 4);
    memset(D, 0, width);

    uchar *D2 = (uchar *) malloc(width * 4);
    memset(D2, 0, width);

    int count = 2000;
    long start = getCurrentMicrosecond();
    for (int i = 0; i < count; i++) {
        test_c(S, width, D, x_ofs);
    }
    float time1 = (getCurrentMicrosecond() - start) / count;

    start = getCurrentMicrosecond();
    for (int i = 0; i < count; i++) {
        test_neon2(S, width, D2, x_ofs);
    }
    float time2 = (getCurrentMicrosecond() - start) / count;
    LOGD("c takes %.2fus, neon takes %.2fus, improve %.2f", time1, time2, 100 * (time1 - time2) / time1);

    int ff = 0;
    for (int i = 0; i < width; i++) {
//        LOGD("%d: %d -> %d", i, D[i], D2[i]);
        if (D[i] != D2[i]) {
            ff++;
        }
    }
    if (ff > 0) {
        LOGE("fuck: %d", ff);
    } else {
        LOGD("All match");

    }
}
