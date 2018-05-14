//
// Created by dingjikerbo on 2018/4/1.
//

#include "main.h"



extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    short A[16] = {
            100, 200, 300, 400, 500, 600, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
    };

    int16x4x2_t d = vld2_s16(A);

    int16x4_t d0 = d.val[0];
    int16x4_t d1 = d.val[1];

    int16x4_t ds = vqdmulh_s16(d0, d1);
    show16(ds);
}
