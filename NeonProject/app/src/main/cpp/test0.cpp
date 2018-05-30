//
// Created by dingjikerbo on 2018/4/1.
//

#include <math.h>
#include <cmath>
#include "main.h"

const int INTER_RESIZE_COEF_BITS = 11;
const int INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS;
const int SHIFT = INTER_RESIZE_COEF_BITS * 2;
const int DELTA = 1 << (SHIFT - 1);

static void
test_neon1(const int **src, uint8_t *dst, const short *beta, int width) {
    int b0 = beta[0], b1 = beta[1];
    const int *S0 = src[0], *S1 = src[1];

    int32x4_t qdelta = vdupq_n_s32(DELTA);
    int32x4_t qshift = vdupq_n_s32(-SHIFT);

    int align_16 = width / 16 * 16, x;

    for (x = 0; x < align_16; x += 16) {
        int32x4_t q00 = vld1q_s32(S0 + x);
        int32x4_t q01 = vld1q_s32(S0 + x + 4);
        int32x4_t q02 = vld1q_s32(S0 + x + 8);
        int32x4_t q03 = vld1q_s32(S0 + x + 12);

        int32x4_t q10 = vld1q_s32(S1 + x);
        int32x4_t q11 = vld1q_s32(S1 + x + 4);
        int32x4_t q12 = vld1q_s32(S1 + x + 8);
        int32x4_t q13 = vld1q_s32(S1 + x + 12);

        q00 = vmulq_n_s32(q00, b0);
        q01 = vmulq_n_s32(q01, b0);
        q02 = vmulq_n_s32(q02, b0);
        q03 = vmulq_n_s32(q03, b0);

        q00 = vmlaq_n_s32(q00, q10, b1);
        q01 = vmlaq_n_s32(q01, q11, b1);
        q02 = vmlaq_n_s32(q02, q12, b1);
        q03 = vmlaq_n_s32(q03, q13, b1);

        q00 = vaddq_s32(q00, qdelta);
        q01 = vaddq_s32(q01, qdelta);
        q02 = vaddq_s32(q02, qdelta);
        q03 = vaddq_s32(q03, qdelta);

        q00 = vshlq_s32(q00, qshift);
        q01 = vshlq_s32(q01, qshift);
        q02 = vshlq_s32(q02, qshift);
        q03 = vshlq_s32(q03, qshift);

        uint16x8_t qt1 = vcombine_u16(vqmovun_s32(q00), vqmovun_s32(q01));
        uint16x8_t qt2 = vcombine_u16(vqmovun_s32(q02), vqmovun_s32(q03));

        vst1q_u8(dst + x, vcombine_u8(vmovn_u16(qt1), vmovn_u16(qt2)));
    }

    for (; x < width; x++) {
        int val = S0[x] * b0 + S1[x] * b1;
        dst[x] = (val + DELTA) >> SHIFT;
    }
}

static void
test_c(const int **src, uint8_t *dst, const short *beta, int width) {
    int b0 = beta[0], b1 = beta[1];
    const int *S0 = src[0], *S1 = src[1];

    for (int x = 0; x < width; x++) {
        int val = S0[x] * b0 + S1[x] * b1;
        dst[x] = (val + DELTA) >> SHIFT;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    srand(1);
    int width = 9999 * 4;

    int max = 255 << 11;

    int *src0 = (int *) malloc(width * sizeof(int));
    for (int i = 0; i < width; i++) {
        src0[i] = rand() % max;
//        src0[i] = max;
    }

    int *src1 = (int *) malloc(width * sizeof(int));
    for (int i = 0; i < width; i++) {
        src1[i] = rand() % max;
//        src1[i] = max;
    }

    int *src[] = {src0, src1};

    short beta[] = {127, 2048 - 127};

    uint8_t *dst1 = (uint8_t *) malloc(width);
    uint8_t *dst2 = (uint8_t *) malloc(width);

    int count = 5000;
    float t1, t2;

#if 1
    {
        long start = getCurrentMicrosecond();
        for (int i = 0; i < count; i++) {
            test_c((const int **) src, dst1, beta, width);
        }
        t1 = (getCurrentMicrosecond() - start) / 1000.0f / count;
        LOGD("test_c takes %.3fms", t1);
    }
#endif

#if 1
    {
        long start = getCurrentMicrosecond();
        for (int i = 0; i < count; i++) {
            test_neon1((const int **) src, dst2, beta, width);
        }
        t2 = (getCurrentMicrosecond() - start) / 1000.0f / count;
        LOGD("test_neon takes %.3fms", t2);

    }
#endif

#if 1
    LOGD("Time reduce %.2f%%", (t1 - t2) * 100 / t1);

    int diff = 0;
    for (int i = 0; i < width; i++) {

//        LOGD("%d: %d -> %d", i, dst1[i], dst2[i]);

        if (dst1[i] != dst2[i]) {
            diff++;
        }
    }
    if (diff != 0) {
        LOGE("Not match, %d", diff);
    } else {
        LOGD("Matched!!");
    }
#endif

}
