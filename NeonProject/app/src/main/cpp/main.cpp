#include "main.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
//    test_sincos();
    int num[] = {0x1, 0x2, 0x7, 0xf};
    int num2[] = {0x4, 0x9, 0xb, 0xe};
    int32x4_t q1 = vld1q_s32(num);
    int32x4_t q2 = vld1q_s32(num2);
    int32x4_t q3 = vtstq_s32(q1, q2);
    show32q(q3);
};