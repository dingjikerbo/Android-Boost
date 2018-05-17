//
// Created by dingjikerbo on 2018/4/1.
//

#include <math.h>
#include <cmath>
#include "main.h"

typedef uint8_t uchar;

void
test_neon(const uchar *S_, int dwidth, uchar *D_, int *x_ofs) {
    int *S = (int *) S_, *D = (int *) D_;

    for (int x = 0; x < dwidth; x += 4) {
        int32x4_t qofs = vld1q_s32(x_ofs + x);

        int ofs0 = vgetq_lane_s32(qofs, 0);
        int ofs1 = vgetq_lane_s32(qofs, 1);
        int ofs2 = vgetq_lane_s32(qofs, 2);
        int ofs3 = vgetq_lane_s32(qofs, 3);

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
test_c(const uchar *S, int dwidth, uchar *D, int *x_ofs) {
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
    int width = 1000000;
    float scale = 0.5f;
    int *x_ofs = (int *) malloc(width * sizeof(int));
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
        test_neon(S, width, D2, x_ofs);
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
