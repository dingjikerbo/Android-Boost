//
// Created by dingjikerbo on 2018/4/1.
//

#include "main.h"

JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
//    float n[] = {
//            1.2f, 2.3f, 3.4f, 4.5f
//    };
//    float32x4_t qf = vld1q_f32(n);
//    qf = vrev64q_f32(qf);
//    show32fq(qf);

//    int16_t n[] = {
//            1, 2, 3, 4, 5, 6, 7, 8
//    };
//    int16x8_t q1 = vld1q_s16(n);
//    q1 = vrev64q_s16(q1);
//    q1 = vcombine_s16(vget_high_s16(q1), vget_low_s16(q1));
//    q1 = vnegq_s16(q1);
//    show16q(q1);

    int r1 = -0, i1 = -47;
    int r2 = -32767, i2 = -0;

    int s1 = r1 * r2 - i1 * i2;
    int s2 = r1 * i2 + r2 * i1;

    LOGD("s1 = %d, s2 = %d", s1>>15, s2>>15);

};

