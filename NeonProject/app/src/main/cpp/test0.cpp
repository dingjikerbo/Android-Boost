//
// Created by dingjikerbo on 2018/4/1.
//

#include "main.h"



extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    short A[8] = {
            1, 2, 3, 4, 5, 6, 7, 8
    };

    int16x4x2_t d = vld2_s16(A);

    int16x4_t d0 = d.val[0];
    int16x4_t d1 = d.val[1];

    show16(d0);

    show16(d1);
}
