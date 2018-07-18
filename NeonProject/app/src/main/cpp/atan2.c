//
// Created by 李文田 on 2018/7/18.
//

//
// Created by dingjikerbo on 2018/4/1.
//


#include "main.h"


float atan2_approximation1(float y, float x) {
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
    //Volkan SALMA

    const float ONEQTR_PI = M_PI / 4.0;
    const float THRQTR_PI = 3.0 * M_PI / 4.0;
    float r, angle;
    float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
    if (x < 0.0f) {
        r = (x + abs_y) / (abs_y - x);
        angle = THRQTR_PI;
    } else {
        r = (x - abs_y) / (x + abs_y);
        angle = ONEQTR_PI;
    }
    angle += (0.1963f * r * r - 0.9817f) * r;
    if (y < 0.0f)
        return (-angle);     // negate if in quad III or IV
    else
        return (angle);
}

#define PI_FLOAT     3.14159265f
#define PIBY2_FLOAT  1.5707963f

// |error| < 0.005
float atan2_approximation2(float y, float x) {
    if (x == 0.0f) {
        if (y > 0.0f) return PIBY2_FLOAT;
        if (y == 0.0f) return 0.0f;
        return -PIBY2_FLOAT;
    }
    float atan;
    float z = y / x;
    if (fabs(z) < 1.0f) {
        atan = z / (1.0f + 0.28f * z * z);
        if (x < 0.0f) {
            if (y < 0.0f) return atan - PI_FLOAT;
            return atan + PI_FLOAT;
        }
    } else {
        atan = PIBY2_FLOAT - z / (z * z + 0.28f);
        if (y < 0.0f) return atan - PI_FLOAT;
    }
    return atan;
}

static const float __atan2f_lut[4] = {
        -0.0443265554792128f,  //p7
        -0.3258083974640975f,  //p3
        +0.1555786518463281f,  //p5
        +0.9997878412794807f   //p1
};
const float __atan2f_pi_2 = M_PI_2;

static inline float atan2f_c(float y, float x) {
    float a, b, c, r, xx;
    int m;
    union {
        float f;
        int i;
    } xinv;

    //fast inverse approximation (2x newton)
    xx = fabs(x);
    xinv.f = xx;
    m = 0x3F800000 - (xinv.i & 0x7F800000);
    xinv.i = xinv.i + m;
    xinv.f = 1.41176471f - 0.47058824f * xinv.f;
    xinv.i = xinv.i + m;
    b = 2.0 - xinv.f * xx;
    xinv.f = xinv.f * b;
    b = 2.0 - xinv.f * xx;
    xinv.f = xinv.f * b;

    c = fabs(y * xinv.f);

    //fast inverse approximation (2x newton)
    xinv.f = c;
    m = 0x3F800000 - (xinv.i & 0x7F800000);
    xinv.i = xinv.i + m;
    xinv.f = 1.41176471f - 0.47058824f * xinv.f;
    xinv.i = xinv.i + m;
    b = 2.0 - xinv.f * c;
    xinv.f = xinv.f * b;
    b = 2.0 - xinv.f * c;
    xinv.f = xinv.f * b;

    //if |x| > 1.0 -> ax = -1/ax, r = pi/2
    xinv.f = xinv.f + c;
    a = (c > 1.0f);
    c = c - a * xinv.f;
    r = a * __atan2f_pi_2;



    //polynomial evaluation
    xx = c * c;
    a = (__atan2f_lut[0] * c) * xx + (__atan2f_lut[2] * c);
    b = (__atan2f_lut[1] * c) * xx + (__atan2f_lut[3] * c);

    xx = xx * xx;
    r = r + a * xx;
    r = r + b;


    //determine quadrant and test for small x.
    b = M_PI;
    b = b - 2.0f * r;
    r = r + (x < 0.0f) * b;

    b = (fabs(x) < 0.000001f);
    c = !b;
    r = c * r;
    r = r + __atan2f_pi_2 * b;
    b = r + r;
    r = r - (y < 0.0f) * b;

    return r;

}

static inline float32x4_t atan2f_v(float32x4_t y, float32x4_t x) {
    float32x4_t q_x_1 = vrecpeq_f32(x);
    float32x4_t q_x_2 = vrecpsq_f32(q_x_1, x);
    q_x_1 = vmulq_f32(q_x_1, q_x_2);
    q_x_2 = vrecpsq_f32(q_x_1, x);
    float32x4_t q_1_x = vmulq_f32(q_x_1, q_x_2); // 1/x


    float32x4_t q_y_1_x = vmulq_f32(y, q_1_x);
    float32x4_t q_pi_2 = vmovq_n_f32(__atan2f_pi_2);
    float32x4_t q_y_1_x_abs = vabsq_f32(q_y_1_x);


    q_x_1 = vrecpeq_f32(q_y_1_x_abs);
    q_x_2 = vrecpsq_f32(q_x_1, q_y_1_x_abs);
    q_x_1 = vmulq_f32(q_x_1, q_x_2);
    q_x_2 = vrecpsq_f32(q_x_1, q_y_1_x_abs);
    float32x4_t q_1_x_2 = vmulq_f32(q_x_1, q_x_2); // 1/x

    q_1_x_2 = vaddq_f32(q_1_x_2, q_y_1_x_abs);
    float32x4_t q_1_0_f = vmovq_n_f32(1.0f);

    uint32x4_t q_temp = vcgtq_f32(q_y_1_x_abs, q_1_0_f);

    q_1_x_2 = vandq_s8(q_1_x_2, q_temp);
    q_y_1_x_abs = vsubq_f32(q_y_1_x_abs, q_1_x_2);
    float32x4_t q_compen = vandq_s8(q_temp, q_pi_2);


    float32x4_t q_p7p3p5p1 = vld1q_f32(__atan2f_lut);

    float32x4_t q_x = q_y_1_x_abs;
    float32x4_t q_xx = vmulq_f32(q_x, q_x);
    float32x4_t q_xxxx = vmulq_f32(q_xx, q_xx);

    float32x4_t q_px1 = vmulq_lane_f32(q_p7p3p5p1, vget_low_f32(q_x), 0);
    float32x4_t q_px2 = vmulq_lane_f32(q_p7p3p5p1, vget_low_f32(q_x), 1);
    float32x4_t q_px3 = vmulq_lane_f32(q_p7p3p5p1, vget_high_f32(q_x), 0);
    float32x4_t q_px4 = vmulq_lane_f32(q_p7p3p5p1, vget_high_f32(q_x), 1);

    float32x2_t d_ab1 = vmla_lane_f32(vget_high_f32(q_px1), vget_low_f32(q_px1), vget_low_f32(q_xx),
                                      0);
    float32x2_t d_ab2 = vmla_lane_f32(vget_high_f32(q_px2), vget_low_f32(q_px2), vget_low_f32(q_xx),
                                      1);
    float32x2_t d_ab3 = vmla_lane_f32(vget_high_f32(q_px3), vget_low_f32(q_px3),
                                      vget_high_f32(q_xx), 0);
    float32x2_t d_ab4 = vmla_lane_f32(vget_high_f32(q_px4), vget_low_f32(q_px4),
                                      vget_high_f32(q_xx), 1);

    float32x4_t q_ab13 = vcombine_f32(d_ab1, d_ab3);
    float32x4_t q_ab24 = vcombine_f32(d_ab2, d_ab4);


    float32x4x2_t q_ab1234 = vtrnq_f32(q_ab13, q_ab24);

    float32x4_t q_a = q_ab1234.val[0];
    float32x4_t q_b = q_ab1234.val[1];

    float32x4_t q_r = vaddq_f32(q_compen, vaddq_f32(vmulq_f32(q_a, q_xxxx), q_b));

    // clear NaN
//    q_temp = vceqq_f32(q_r, q_r);
//    q_r = vandq_s8(q_temp, q_r);    // clear NaN

    float32x4_t q_pi = vmovq_n_f32(M_PI);

    float32x4_t q_2r = vaddq_f32(q_r, q_r);
    float32x4_t q_b_temp = vsubq_f32(q_pi, q_2r);

    float32x4_t q_zero = vmovq_n_f32(0.0f);
    q_temp = vcltq_f32(x, q_zero);
    q_b_temp = vandq_s8(q_b_temp, q_temp);
    q_r = vaddq_f32(q_r, q_b_temp);

    // for nan handle
    float32x4_t q_x_abs = vabsq_f32(x);

    // to control the Accuracy
    float32x4_t q_000001 = vmovq_n_f32(0.00000001f);

    q_temp = vcgtq_f32(q_x_abs, q_000001);
    q_r = vandq_s8(q_r, q_temp);

    q_temp = vmvnq_u32(q_temp);

    q_r = vaddq_f32(q_r, vandq_s8(q_pi_2, q_temp));

    q_b_temp = vaddq_f32(q_r, q_r);

    q_temp = vcltq_f32(y, q_zero);
    q_b_temp = vandq_s8(q_b_temp, q_temp);
    q_r = vsubq_f32(q_r, q_b_temp);

    return q_r;
}

static inline float fast_atan2(float y, float x) {
    static const float c1 = M_PI / 4.0;
    static const float c2 = M_PI * 3.0 / 4.0;
    if (y == 0 && x == 0)
        return 0;
    float abs_y = fabsf(y);
    float angle;
    if (x >= 0)
        angle = c1 - c1 * ((x - abs_y) / (x + abs_y));
    else
        angle = c2 - c1 * ((x + abs_y) / (-x + abs_y));
    if (y < 0)
        return -angle;
    return angle;
}

static inline float fast_atan2_2(float y, float x) {
    if (x == 0 && y == 0) {
        return 0;
    }

    // 7th order polynomial approximation of atan(z) on [-1,1], slightly
    // tweaked to remove a multiply at the cost of very slightly higher
    // error.
    float a = fminf(fabsf(x), fabsf(y)) / fmaxf(fabsf(x), fabsf(y));
    float s = a * a;
    float r = (((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s + 1.0f) * a;

    if (fabsf(y) > fabsf(x)) r = (float) M_PI_2 - r;
    if (x < 0) r = (float) M_PI - r;
    if (y < 0) r = -r;

    return r;
}

static inline float fast_atan2_2_boost(float y, float x) {
    float a = fminf(fabsf(x), fabsf(y)) / fmaxf(fabsf(x), fabsf(y));
    float s = a * a;
    float r = (((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s + 1.0f) * a;

    if (fabsf(y) > fabsf(x)) r = (float) M_PI_2 - r;
    if (x < 0) r = (float) M_PI - r;
    if (y < 0) r = -r;

    return r;
}

void test_atan2() {
    int nfft = 2048;

    srand(time(NULL));

    float *fy = malloc(nfft * sizeof(float));
    float *fx = malloc(nfft * sizeof(float));

    for (int i = 0; i < nfft; i++) {
        fy[i] = 16.0f / (rand() % 256 - 128 + 1e-4);
        fx[i] = 4.0f / (rand() % 256 - 128 + 1e-4);
    }

    float *f1 = malloc(nfft * sizeof(float));
    float *f2 = malloc(nfft * sizeof(float));
    float *f3 = malloc(nfft * sizeof(float));

    int time1 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f1[i] = atan2f(fy[i], fx[i]);
    }

    int time2 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i += 4) {
        float32x4_t qy = vld1q_f32(fy + i);
        float32x4_t qx = vld1q_f32(fx + i);
        float32x4_t qs = atan2f_v(qy, qx);
        vst1q_f32(f2 + i, qs);
    }
    int time3 = getCurrentMicrosecond();

//    for (int i = 0; i < nfft; i += 4) {
//        float32x4_t qy = vld1q_f32(fy + i);
//        float32x4_t qx = vld1q_f32(fx + i);
//        float32x4_t qs = fast_atan2_2_neon(qy, qx);
//        vst1q_f32(f3 + i, qs);
//    }
    for (int i = 0; i < nfft; i++) {
        f3[i] = fast_atan2_2_boost(fy[i], fx[i]);
    }
    int time4 = getCurrentMicrosecond();

    float sum1 = 0.0f, sum2 = 0.0f;
    for (int i = 0; i < nfft; i++) {
        sum1 += fabsf(100 * (f2[i] - f1[i]) / f1[i]);
        sum2 += fabsf(100 * (f3[i] - f1[i]) / f1[i]);
    }

    LOGD("atan2f_c: %.4f%%, fast_atan2_2_neon: %.4f%%",
         sum1 / nfft, sum2 / nfft);

    LOGD("atan2f = %dus, atan2f_neon= %dus, fast_atan2_2_neon=%dus",
         time2 - time1, time3 - time2, time4 - time3);

//    for (int i = 0; i < nfft; i++) {
//        LOGD("%d f1=%.4f, f2=%.4f, f3=%.4f", i, f1[i], f2[i], f3[i]);
//    }
}




