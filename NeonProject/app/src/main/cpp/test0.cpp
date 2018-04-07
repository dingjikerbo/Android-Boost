//
// Created by dingjikerbo on 2018/4/1.
//

#include "main.h"



extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    int8_t A[8] = {
            4, 8, 3, 1, 5, 6, 7, 8
    };

    int8x8_t d = vld1_s8(A);

	int8_t *B = (int8_t *) malloc(8);
	vst1_lane_s32(B, d, 0);

	for (int i = 0; i < 8; i++) {
		LOGD("B %d = %d", i, B[i]);
	}
}
