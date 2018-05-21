//
// Created by dingjikerbo on 2018/4/1.
//

#include <math.h>
#include <cmath>
#include "main.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    int a[] = {
            0x10, 0x101, 3, 400, 5, 6, 7, 8
    };
    int b[] = {
            0x111, 0x10, 30, 40, 50, 60, 70, 80
    };
    int32x4_t aa = vld1q_s32(a);
    int32x4_t bb = vld1q_s32(b);
    int32x4_t cc = vcgeq_s32(aa, bb);

    int32x4_t uu = vdupq_n_s32(-1);

    int32x4_t ff = vbslq_s32(cc, aa, uu);
    show32q(ff);
}
