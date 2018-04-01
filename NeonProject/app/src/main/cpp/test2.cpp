//
// Created by dingjikerbo on 2018/4/1.
//
#include "main.h"

#define CYCLE 100

static void
test_c(jbyte *A, jbyte *B, int len, int c1, int c2, int c3, int c4) {
	int step = len / 4;
	for (int i = 0; i < step; i++) {
		jbyte *p1 = B + i;
		jbyte *p2 = p1 + step;
		jbyte *p3 = p2 + step;
		jbyte *p4 = p3 + step;

		int sum = *p1 * c1 + *p2 * c2 + *p3 * c3 + *p4 * c4;
		A[i] = sum >> 2;
	}
}

#define MULTI(q, p) {\
    int16x8_t qq1 = vmovl_s8(vget_low_s8(q)); \
    int16x8_t qq2 = vmovl_s8(vget_high_s8(q)); \
    int32x4_t qt1 = vmovl_s16(vget_low_s16(qq1)); \
    int32x4_t qt2 = vmovl_s16(vget_high_s16(qq1)); \
    int32x4_t qt3 = vmovl_s16(vget_low_s16(qq2)); \
    int32x4_t qt4 = vmovl_s16(vget_high_s16(qq2)); \
    qs1 = vmlaq_n_s32(qs1, qt1, p); \
    qs2 = vmlaq_n_s32(qs2, qt2, p); \
    qs3 = vmlaq_n_s32(qs3, qt3, p); \
    qs4 = vmlaq_n_s32(qs4, qt4, p); \
}\

static
int8x16_t combine(int32x4_t q1, int32x4_t q2, int32x4_t q3, int32x4_t q4) {
	int16x4_t d1 = vshrn_n_s32(q1, 2);
	int16x4_t d2 = vshrn_n_s32(q2, 2);
	int16x4_t d3 = vshrn_n_s32(q3, 2);
	int16x4_t d4 = vshrn_n_s32(q4, 2);

	int8x8_t ds1 = vmovn_s16(vcombine_s16(d1, d2));
	int8x8_t ds2 = vmovn_s16(vcombine_s16(d3, d4));

	return vcombine_s8(ds1, ds2);
}

static void
test_neon(jbyte *A, jbyte *B, int len, int c1, int c2, int c3, int c4) {
	int step = len >> 2;
	for (int i = 0; i < step; i += 16) {
		int8x16_t q1 = vld1q_s8(B);
		int8x16_t q2 = vld1q_s8(B + step);
		int8x16_t q3 = vld1q_s8(B + step * 2);
		int8x16_t q4 = vld1q_s8(B + step * 3);

		int32x4_t qs1 = vdupq_n_s32(0);
		int32x4_t qs2 = vdupq_n_s32(0);
		int32x4_t qs3 = vdupq_n_s32(0);
		int32x4_t qs4 = vdupq_n_s32(0);

		MULTI(q1, c1);
		MULTI(q2, c2);
		MULTI(q3, c3);
		MULTI(q4, c4);

		vst1q_s8(A + i, combine(qs1, qs2, qs3, qs4));
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester2_testNeon(JNIEnv *env, jobject thiz, jbyteArray A, jbyteArray B,
                                              jint len,
                                              jint c1, jint c2, jint c3, jint c4) {
	jbyte *AA = env->GetByteArrayElements(A, 0);
	jbyte *BB = env->GetByteArrayElements(B, 0);

	int duration = 0;

	for (int i = 0; i < CYCLE; i++) {
		long start = getCurrentMillisecond();
		test_neon(AA, BB, len, c1, c2, c3, c4);
		duration += getCurrentMillisecond() - start;
	}

	LOGD("testNeon >>> takes %dms, ", duration);

	env->ReleaseByteArrayElements(A, AA, 0);
	env->ReleaseByteArrayElements(B, BB, 0);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester2_testC(JNIEnv *env, jobject thiz, jbyteArray A, jbyteArray B,
                                           jint len,
                                           jint c1, jint c2, jint c3, jint c4) {
	jbyte *AA = env->GetByteArrayElements(A, 0);
	jbyte *BB = env->GetByteArrayElements(B, 0);

	long duration = 0;

	for (int i = 0; i < CYCLE; i++) {
		long start = getCurrentMillisecond();
		test_c(AA, BB, len, c1, c2, c3, c4);
		duration += getCurrentMillisecond() - start;
	}

	LOGD("testC >>> takes %dms, ", duration);

	env->ReleaseByteArrayElements(A, AA, 0);
	env->ReleaseByteArrayElements(B, BB, 0);
}