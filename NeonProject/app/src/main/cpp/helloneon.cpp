#include "helloneon.h"

int test_normal(int *arr, int len) {
	unsigned int acc = 0;
	for (int i = 0; i < len; i++) {
		acc += arr[i]; // a + b + c + d + e + f + g + h
	}
	return acc;
}

int test_neon(int *ptr, int items) {
	uint32_t result;
	uint32x2_t vec64a, vec64b;
	uint32x4_t vec128 = vdupq_n_s32(0); // clear accumulators
	for (int *i = ptr; i < ptr + items; i += 4) {
		uint32x4_t temp128 = vld1q_s32(i); // load 4x 32 bit values
		vec128 = vaddq_s32(vec128, temp128); // add 128 bit vectors
	}
	vec64a = vget_low_s32(vec128);  // split 128 bit vector
	vec64b = vget_high_s32(vec128); //  into 2x 64 bit vectors
	vec64a = vadd_s32(vec64a, vec64b); // add 64 bit vectors together
	result = vget_lane_s32(vec64a, 0); // extract lanes and
	result += vget_lane_s32(vec64a, 1); //  add together scalars
	return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_MainActivity_mytest(JNIEnv *env, jobject thiz, jintArray A, jint len) {
	jint *AA = env->GetIntArrayElements(A, 0);

	int times = 10;
	int duration = 0;
	int sum = 0;

	for (int i = 0; i < times; i++) {
		long start = getCurrentMillisecond();
		sum = test_normal(AA, len);
		duration += getCurrentMillisecond() - start;
	}

	LOGD("C >>> sum = %d, takes %dms, ", sum, duration);

	for (int i = 0; i < times; i++) {
		long start = getCurrentMillisecond();
		sum = test_neon(AA, len);
		duration += getCurrentMillisecond() - start;
	}

	LOGD("Neon >>> sum = %d, takes %dms, ", sum, duration);

	env->ReleaseIntArrayElements(A, AA, 0);
}

short B[8] = {
        1, 2, 3, 4, 5, 6, 7, 8
};

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_MainActivity_testInstruction(JNIEnv *env, jobject thiz) {
    int8_t *bottom_base = new int8_t[16] {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
    };
//    int8x16_t Q = vld1q_s8(bottom_base);
//    int8x16_t Q = vdupq_n_s8(3);
    int8x16_t Q = vmovq_n_s8(3);
    show8q(Q);
}
