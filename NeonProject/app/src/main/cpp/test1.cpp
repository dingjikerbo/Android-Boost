//
// Created by dingjikerbo on 2018/4/1.
//
#include "main.h"

static int test_c(jint *A, jint *B, int len) {
	unsigned int acc = 0;
	for (int i = 0; i < len; i++) {
		acc += A[i] * B[i];  // a + b + c + d + e + f + g + h
	}
	return acc;
}

static int test_neon(int *A, int *B, int len) {
	int align4 = len & ~3;
	int32x4_t qs = vdupq_n_s32(0);
	for (int i = 0; i < align4; i += 4) {
		int32x4_t qA = vld1q_s32(A + i);
		int32x4_t qB = vld1q_s32(B + i);
		qs = vmlaq_s32(qs, qA, qB);
	}
	int32x2_t qh = vget_high_s32(qs);
	int32x2_t ql = vget_low_s32(qs);
	int32x2_t qu = vpadd_s32(qh, ql);
	qu = vpadd_s32(qu, qu);
	return vget_lane_s32(qu, 0);
}

static int test(int *AA, int *BB, int len) {
	int times = 1000;
	int duration = 0;
	int sum = 0;

	for (int i = 0; i < times; i++) {
		long start = getCurrentMillisecond();
		sum = test_c(AA, BB, len);
		duration += getCurrentMillisecond() - start;
	}

	LOGD("C >>> sum = %d, takes %dms, ", sum, duration);

	duration = 0;

	for (int i = 0; i < times; i++) {
		long start = getCurrentMillisecond();
		sum = test_neon(AA, BB, len);
		duration += getCurrentMillisecond() - start;
	}

	LOGD("Neon >>> sum = %d, takes %dms, ", sum, duration);

	return sum;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester1_testNeon(JNIEnv *env, jobject thiz, jintArray A, jintArray B, jint len) {
	jint *AA = env->GetIntArrayElements(A, 0);
	jint *BB = env->GetIntArrayElements(B, 0);

	test(AA, BB, len);

	env->ReleaseIntArrayElements(A, AA, 0);
	env->ReleaseIntArrayElements(B, BB, 0);
}