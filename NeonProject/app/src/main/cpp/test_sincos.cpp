//
// Created by 李文田 on 2018/7/23.
//
#include "main.h"
#include "cv_neon_mathfun.h"

typedef float32x4_t v4sf;  // vector of 4 float
typedef uint32x4_t v4su;  // vector of 4 uint32
typedef int32x4_t v4si;  // vector of 4 uint32

#define c_minus_cephes_DP1 -0.78515625
#define c_minus_cephes_DP2 -2.4187564849853515625e-4
#define c_minus_cephes_DP3 -3.77489497744594108e-8
#define c_sincof_p0 -1.9515295891E-4
#define c_sincof_p1  8.3321608736E-3
#define c_sincof_p2 -1.6666654611E-1
#define c_coscof_p0  2.443315711809948E-005
#define c_coscof_p1 -1.388731625493765E-003
#define c_coscof_p2  4.166664568298827E-002
#define c_cephes_FOPI 1.27323954473516 // 4 / M_PI

void sincos_ps1(v4sf x, v4sf *ysin, v4sf *ycos) {
    v4sf xmm1, xmm2, xmm3, y;

    v4su emm2;

    v4su sign_mask_sin, sign_mask_cos;
    sign_mask_sin = vcltq_f32(x, vdupq_n_f32(0));
    x = vabsq_f32(x);

    /* scale by 4/Pi */
    y = vmulq_f32(x, vdupq_n_f32(c_cephes_FOPI));

    /* store the integer part of y in mm0 */
    emm2 = vcvtq_u32_f32(y);
    /* j=(j+1) & (~1) (see the cephes sources) */
    emm2 = vaddq_u32(emm2, vdupq_n_u32(1));
    emm2 = vandq_u32(emm2, vdupq_n_u32(~1));
    y = vcvtq_f32_u32(emm2);

    /* get the polynom selection mask
       there is one polynom for 0 <= x <= Pi/4
       and another one for Pi/4<x<=Pi/2

       Both branches will be computed.
    */
    v4su poly_mask = vtstq_u32(emm2, vdupq_n_u32(2));

    /* The magic pass: "Extended precision modular arithmetic"
       x = ((x - y * DP1) - y * DP2) - y * DP3; */
    xmm1 = vmulq_n_f32(y, c_minus_cephes_DP1);
    xmm2 = vmulq_n_f32(y, c_minus_cephes_DP2);
    xmm3 = vmulq_n_f32(y, c_minus_cephes_DP3);
    x = vaddq_f32(x, xmm1);
    x = vaddq_f32(x, xmm2);
    x = vaddq_f32(x, xmm3);

    sign_mask_sin = veorq_u32(sign_mask_sin, vtstq_u32(emm2, vdupq_n_u32(4)));
    sign_mask_cos = vtstq_u32(vsubq_u32(emm2, vdupq_n_u32(2)), vdupq_n_u32(4));

    /* Evaluate the first polynom  (0 <= x <= Pi/4) in y1,
       and the second polynom      (Pi/4 <= x <= 0) in y2 */
    v4sf z = vmulq_f32(x,x);
    v4sf y1, y2;

    y1 = vmulq_n_f32(z, c_coscof_p0);
    y2 = vmulq_n_f32(z, c_sincof_p0);
    y1 = vaddq_f32(y1, vdupq_n_f32(c_coscof_p1));
    y2 = vaddq_f32(y2, vdupq_n_f32(c_sincof_p1));
    y1 = vmulq_f32(y1, z);
    y2 = vmulq_f32(y2, z);
    y1 = vaddq_f32(y1, vdupq_n_f32(c_coscof_p2));
    y2 = vaddq_f32(y2, vdupq_n_f32(c_sincof_p2));
    y1 = vmulq_f32(y1, z);
    y2 = vmulq_f32(y2, z);
    y1 = vmulq_f32(y1, z);
    y2 = vmulq_f32(y2, x);
    y1 = vsubq_f32(y1, vmulq_f32(z, vdupq_n_f32(0.5f)));
    y2 = vaddq_f32(y2, x);
    y1 = vaddq_f32(y1, vdupq_n_f32(1));

    /* select the correct result from the two polynoms */
    v4sf ys = vbslq_f32(poly_mask, y1, y2);
    v4sf yc = vbslq_f32(poly_mask, y2, y1);
    *ysin = vbslq_f32(sign_mask_sin, vnegq_f32(ys), ys);
    *ycos = vbslq_f32(sign_mask_cos, yc, vnegq_f32(yc));
}

static const float constants[]  = {
        /* q1 */  0,                M_PI_2,           0,                M_PI_2,
        /* q2 */  M_PI,             M_PI,             M_PI,             M_PI,
        /* q3 */  4.f/M_PI,         4.f/M_PI,         4.f/M_PI,         4.f/M_PI,
        /* q4 */ -4.f/(M_PI*M_PI), -4.f/(M_PI*M_PI), -4.f/(M_PI*M_PI), -4.f/(M_PI*M_PI),
        /* q5 */  2.f,              2.f,              2.f,              2.f,
        /* q6 */  .225f,            .225f,            .225f,            .225f
};

static void vsincos2(const float angles[2], float results[4]) {
    asm volatile(
    // Load q0 with [angle1,angle1,angle2,angle2]
    "vldmia %1, { d3 }\n\t"
            "vdup.f32 d0, d3[0]\n\t"
            "vdup.f32 d1, d3[1]\n\t"
            // Load q1-q6 with constants
            "vldmia %2, { q1-q6 }\n\t"
            // Cos(x) = Sin(x+PI/2), so
            // q0 = [angle1, angle1+PI/2, angle2, angle2+PI/2]
            "vadd.f32 q0,q0,q1\n\t"
            // if angle1+PI/2>PI, substract 2*PI
            // q0-=(q0>PI)?2*PI:0
            "vcge.f32 q1,q0,q2\n\t"
            "vand.f32 q1,q1,q2\n\t"
            "vmls.f32 q0,q1,q5\n\t"
            // q0=(4/PI)*q0 - q0*abs(q0)*4/(PI*PI)
            "vabs.f32 q1,q0\n\t"
            "vmul.f32 q1,q0,q1\n\t"
            "vmul.f32 q0,q0,q3\n\t"
            "vmul.f32 q1,q1,q4\n\t"
            "vadd.f32 q0,q0,q1\n\t"
            // q0+=.225*(q0*abs(q0) - q0)
            "vabs.f32 q1,q0\n\t"
            "vmul.f32 q1,q0,q1\n\t"
            "vsub.f32 q1,q0\n\t"
            "vmla.f32 q0,q1,q6\n\t"
            "vstmia %0, { q0 }\n\t"
    :: "r"(results), "r"(angles), "r"(constants)
    : "memory","cc","q0","q1","q2","q3","q4","q5","q6"
    );
}

static void sincos3() {

}

#define RUN_RESULT(s, func) do {\
start = getCurrentMicrosecond();\
for (int i = 0; i<nfft; i++) { \
out0[i] = func(angles[i]); \
}\
end = getCurrentMicrosecond();\
LOGD("%s takes %dus", s, end-start);\
} while(0)

#define RUN_RESULT_VECTOR(s, sinfunc, ...) do {\
start = getCurrentMicrosecond();\
for (int i = 0; i<nfft; i+=4) { \
float32x4_t qf = vld1q_f32(angles+i);\
sinfunc(qf, __VA_ARGS__); \
vst1q_f32(out1+i, qsin); \
}\
end = getCurrentMicrosecond();\
LOGD("%s takes %dus", s, end-start);\
} while(0)

#define TEST_RESULT() do{\
sum=0.0f, max=0.0f;\
for (int i = 0; i < nfft; i++) {\
float diff = getDiff(out0[i], out1[i]);\
/*LOGD(">>> [%d] angle=%.4f out=%.4f ---> %.4f, diff = %.4f%%", i, angles[i], out0[i], out1[i], diff*100);*/\
max = fmaxf(max, diff);\
sum += 100 * diff;\
}\
LOGD("Func1: error = %.4f%%, max = %.4f%%", sum/nfft, max*100);\
} while(0)

void test_sincos() {
    int nfft = 2048, start, end;
    float32x4_t qsin, qcos;
    float sum, max;

    float *angles = static_cast<float *>(malloc(nfft * sizeof(float)));
    float *out0 = static_cast<float *>(malloc(nfft * sizeof(float)));
    float *out1 = static_cast<float *>(malloc(nfft * sizeof(float)));

    for (int i = 0; i < nfft; i++) {
        angles[i] = -M_PI + 2 * i * M_PI/nfft;
    }

    RUN_RESULT("sinf", sinf);

    RUN_RESULT_VECTOR("sincos_ps1", sincos_ps1, &qsin, &qcos);
    TEST_RESULT();

    RUN_RESULT_VECTOR("cv_maths::sincos_ps", cv_maths::sincos_ps, &qsin, &qcos);
    TEST_RESULT();

    float results[4];
    start = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i += 2) {
        vsincos2(angles+i, results);
        out1[i] = results[0];
        out1[i+1] = results[2];
    }
    end = getCurrentMicrosecond();
    LOGD("last takes %dus", end-start);

    TEST_RESULT();
}