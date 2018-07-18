#include "main.h"

typedef struct {
    int16_t r;
    int16_t i;
} cpxi;

typedef struct {
    float r;
    float i;
} cpxf;

void test1(cpxi *input, int nfft, float *magOut, float *phaseOut) {
    const int hs = nfft / 2 + 1, hs8=hs&(~7);
    float kr = nfft / 32767.f;

    for (int i = 0; i < hs8; i += 8) {
        int16x8x2_t qi = vld2q_s16((int16_t*)(input + i));
        int16x4_t qa0 = vget_low_s16(qi.val[0]);
        int16x4_t qa1 = vget_high_s16(qi.val[0]);
        int16x4_t qb0 = vget_low_s16(qi.val[1]);
        int16x4_t qb1 = vget_high_s16(qi.val[1]);

        int32x4_t qs0 = vmull_s16(qa0, qa0);
        int32x4_t qs1 = vmull_s16(qa1, qb1);

        qs0 = vmlal_s32(qs0, qb0, qb0);
        qs1 = vmlal_s32(qs1, qb1, qb1);

        float32x4_t qf0 = vcvtq_f32_s32(qs0);
        float32x4_t qf1 = vcvtq_f32_s32(qs1);

        qf0 = sqrtv(qf0);
        qf1 = sqrtv(qf1);

        qf0 = vmulq_n_f32(qf0, kr);
        qf1 = vmulq_n_f32(qf1, kr);

        vst1q_f32(magOut + i, qf0);
        vst1q_f32(magOut + i + 4, qf1);

        qf0 = fast_atan2f_neon(vmovl_s16(qb0), vmovl_s16(qa0));
        qf1 = fast_atan2f_neon(vmovl_s16(qb1), vmovl_s16(qa1));
        vst1q_f32(phaseOut + i, qf0);
        vst1q_f32(phaseOut + i + 4, qf1);
    }
}

void test2(cpxi *input, int nfft, float *magOut, float *phaseOut) {
    const int hs = nfft / 2 + 1;
    float kr = nfft / 32767.f;

    cpxf *m_fpacked = malloc(hs * sizeof(cpxf));

    for (int i = 0; i < hs; i++) {
        m_fpacked[i].r = input[i].r * kr;
        m_fpacked[i].i = input[i].i * kr;
    }

    for (int i = 0; i < hs; ++i) {
        float f_r = m_fpacked[i].r;
        float f_i = m_fpacked[i].i;
        magOut[i] = sqrtf(f_r * f_r + f_i * f_i);
        phaseOut[i] = atan2f(f_i, f_r);
    }
}

JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
//    int nfft = 2048, hs = nfft/2+1;
//    cpxi *input = malloc(hs * sizeof(cpxi));
//    srand(0);
//    for (int i = 0; i < hs; i++) {
//        input[i].r = rand() % 2048 - 1024;
//        input[i].i = rand() % 2048 - 1024;
//    }
//    float *mag1 = malloc(sizeof(float) * hs);
//    float *phase1 = malloc(sizeof(float) * hs);
//    float *mag2 = malloc(sizeof(float) * hs);
//    float *phase2 = malloc(sizeof(float) * hs);
//    test1(input, nfft, mag1, phase1);
//    test2(input, nfft, mag2, phase2);
//
//    for (int i = 0; i < hs; i++) {
//        LOGD("%d input: %d, %d  output: %.4f,%.4f --> %.4f,%.4f", i, input[i].r, input[i].i, mag1[i], phase1[i], mag2[i], phase2[i]);
//    }
//    test_atan2();

    float n1[] = {
            1.0f, 2.0f, 3.0f, 4.0f
    };

    float n2[] ={
        -2.0f, 1.0f, -3.0f, 5.0f
    };

    float32x4_t qf1 = vld1q_f32(n1);
    float32x4_t qf2 = vld1q_f32(n2);
    float32x4_t qf3 = vmaxq_f32(qf1, qf2);
    show32fq(qf3);
};