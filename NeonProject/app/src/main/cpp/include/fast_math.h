//
// Created by 李文田 on 2018/7/23.
//

#ifndef NEONPROJECT_MATH_H
#define NEONPROJECT_MATH_H

#include <arm_neon.h>
#include <math.h>

static inline float getDiff(float a, float b) {
    float diff = fabsf(fabsf(a) - fabsf(b));
    if (diff < 1e-5) {
        return 0.0f;
    }
    return diff / fabsf(a);
}

static inline float32x4_t invsqrtv(float32x4_t x) {
    float32x4_t sqrt_reciprocal = vrsqrteq_f32(x);
    return vrsqrtsq_f32(x * sqrt_reciprocal, sqrt_reciprocal) * sqrt_reciprocal;
}

// approximative quadword float square root
static inline float32x4_t sqrtv(float32x4_t x) {
    float32x4_t q_inv = x * invsqrtv(x);

    float32x4_t q_zero = vmovq_n_f32(0.00000001f);
    float32x4_t q_abs = vabsq_f32(q_inv);
    uint32x4_t q_temp = vcgtq_f32(q_abs, q_zero);
    q_inv = vandq_s8(q_inv, q_temp);
    return q_inv;
}

static inline float fast_atan2_2_boost2(float y, float x) {
    float a = fminf(fabsf(x), fabsf(y)) / fmaxf(fabsf(x), fabsf(y));
    float s = a * a;

    float r = (0.15931422f * s * s + 1.0f - 0.327622764f * s) * a;

    if (fabsf(y) > fabsf(x)) r = (float) M_PI_2 - r;
    if (x < 0) r = (float) M_PI - r;
    if (y < 0) r = -r;

    return r;
}

static inline float32x4_t fast_atan2f_neon(float32x4_t qy, float32x4_t qx) {
    float32x4_t qabsx = vabsq_f32(qx);
    float32x4_t qabsy = vabsq_f32(qy);

    float32x4_t qmax = vmaxq_f32(qabsx, qabsy);
    float32x4_t qmin = vminq_f32(qabsx, qabsy);
    float32x4_t qmaxr = vrecpeq_f32(qmax);

    float32x4_t qa = vmulq_f32(qmin, qmaxr);
    float32x4_t qs = vmulq_f32(qa, qa);

    float32x4_t qr = vmulq_f32(vdupq_n_f32(0.15931422f), qs);
    float32x4_t qr2 = vmlaq_f32(vdupq_n_f32(1.0f), vdupq_n_f32(-0.327622764f), qs);

    uint32x4_t qrel = vcgeq_f32(qabsy, qabsx);

    qr = vmlaq_f32(qr2, qr, qs);

    float32x4_t qzero = vdupq_n_f32(0.0f);
    int32x4_t qt1 = vcleq_f32(qx, qzero);
    int32x4_t qt2 = vcleq_f32(qy, qzero);

    qr = vmulq_f32(qr, qa);

    qr = vbslq_f32(qrel, vsubq_f32(vdupq_n_f32(M_PI_2), qr), qr);
    qr = vbslq_f32(qt1, vsubq_f32(vdupq_n_f32(M_PI), qr), qr);
    qr = vbslq_f32(qt2, vnegq_f32(qr), qr);

    return qr;
}

#endif //NEONPROJECT_MATH_H
