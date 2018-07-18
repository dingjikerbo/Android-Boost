//
// Created by dingjikerbo on 2018/4/1.
//

#include <time.h>
#include "main.h"
#include <math.h>

float atan2_approximation1(float y, float x)
{
    //http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
    //Volkan SALMA

    const float ONEQTR_PI = M_PI / 4.0;
    const float THRQTR_PI = 3.0 * M_PI / 4.0;
    float r, angle;
    float abs_y = fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition
    if ( x < 0.0f )
    {
        r = (x + abs_y) / (abs_y - x);
        angle = THRQTR_PI;
    }
    else
    {
        r = (x - abs_y) / (x + abs_y);
        angle = ONEQTR_PI;
    }
    angle += (0.1963f * r * r - 0.9817f) * r;
    if ( y < 0.0f )
        return( -angle );     // negate if in quad III or IV
    else
        return( angle );
}

#define PI_FLOAT     3.14159265f
#define PIBY2_FLOAT  1.5707963f
// |error| < 0.005
float atan2_approximation2( float y, float x )
{
    if ( x == 0.0f )
    {
        if ( y > 0.0f ) return PIBY2_FLOAT;
        if ( y == 0.0f ) return 0.0f;
        return -PIBY2_FLOAT;
    }
    float atan;
    float z = y/x;
    if ( fabs( z ) < 1.0f )
    {
        atan = z/(1.0f + 0.28f*z*z);
        if ( x < 0.0f )
        {
            if ( y < 0.0f ) return atan - PI_FLOAT;
            return atan + PI_FLOAT;
        }
    }
    else
    {
        atan = PIBY2_FLOAT - z/(z*z + 0.28f);
        if ( y < 0.0f ) return atan - PI_FLOAT;
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

static inline float atan2f_c(float y, float x)
{
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

static inline float fast_atan2(float y, float x){
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
    if (x == 0 && y== 0) {
        return 0;
    }

    // 7th order polynomial approximation of atan(z) on [-1,1], slightly
    // tweaked to remove a multiply at the cost of very slightly higher
    // error.
    float a = fminf(fabsf(x),fabsf(y))/fmaxf(fabsf(x),fabsf(y));
    float s = a*a;
    float r = ((-0.0464964749f*s + 0.15931422f)*s - 0.327622764f)*s*a + a;

    if (fabsf(y) > fabsf(x)) r = (float)M_PI_2 - r;
    if (x < 0)           r = (float)M_PI   - r;
    if (y < 0)           r =               - r;

    return r;
}

static inline float fast_atan2_2_boost(float y, float x) {
//    if (x == 0 && y== 0) {
//        return 0;
//    }

    int signy = y > 0 ? 1 : -1;
    int signx = x > 0 ? 1 : -1;

    float absy = y * signy;
    float absx = x * signx;

    float a = fminf(absx, absy) / fmaxf(absx, absy);

    float s = a * a;

    float r = ((-0.0464964749f*s + 0.15931422f)*s - 0.327622764f)*s*a + a;

    if (absy > absx) r = (float)M_PI_2 - r;
    if (x < 0)           r = (float)M_PI   - r;
    if (y < 0)           r =               - r;

    return r;
}

static inline float fast_atan2_2_neon(float32x4_t qy, float32x4_t qx) {
    int signy = y > 0 ? 1 : -1;
    int signx = x > 0 ? 1 : -1;

    float absy = y * signy;
    float absx = x * signx;

    float a = fminf(absx, absy) / fmaxf(absx, absy);

    float s = a * a;

    float r = ((-0.0464964749f*s + 0.15931422f)*s - 0.327622764f)*s*a + a;

    if (absy > absx) r = (float)M_PI_2 - r;
    if (x < 0)           r = (float)M_PI   - r;
    if (y < 0)           r =               - r;

    return r;
}


JNIEXPORT void JNICALL
Java_com_example_inuker_neon_Tester0_testInstruction(JNIEnv *env, jobject thiz) {
    int nfft = 1024;
    srand(time(NULL));
    float *fy = malloc(nfft * sizeof(float));
    float *fx = malloc(nfft * sizeof(float));

    float *f1 = malloc(nfft * sizeof(float));
    float *f2 = malloc(nfft * sizeof(float));
    float *f3 = malloc(nfft * sizeof(float));
    float *f4 = malloc(nfft * sizeof(float));
    float *f5 = malloc(nfft * sizeof(float));
    float *f6 = malloc(nfft * sizeof(float));

    for (int i = 0; i < nfft; i++) {
        fy[i] = 4.0f / (rand() % 256 - 128 + 1e-4);
        fx[i] = 4.0f / (rand() % 256 - 128 + 1e-4);
    }

    int time1 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f1[i] = atan2f(fy[i], fx[i]);
    }

    int time2 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f2[i] = atan2_approximation1(fy[i], fx[i]);
    }

    int time3 = getCurrentMicrosecond();
    for (int i = 0; i < nfft; i++) {
        f3[i] = atan2_approximation2(fy[i], fx[i]);
    }
    int time4 = getCurrentMicrosecond();

    for (int i = 0; i < nfft; i++) {
        f4[i] = atan2f_c(fy[i], fx[i]);
    }
    int time5 = getCurrentMicrosecond();

    for (int i = 0; i < nfft; i++) {
        f5[i] = fast_atan2(fy[i], fx[i]);
    }
    int time6 = getCurrentMicrosecond();

    for (int i = 0; i < nfft; i++) {
        f6[i] = fast_atan2_2_boost(fy[i], fx[i]);
    }
    int time7 = getCurrentMicrosecond();

    float sum1 = 0.0f, sum2 = 0.0f, sum3 = 0.0f, sum4 = 0.0f, sum5 = 0.0f;
    for (int i = 0; i < nfft; i++) {
        float k1 = f1[i], k2 = f2[i], k3 = f3[i], k4 = f4[i], k5 = f5[i], k6 = f6[i];
        sum1 += fabsf(100*(k2-k1)/k1);
        sum2 += fabsf(100*(k3-k1)/k1);
        sum3 += fabsf(100*(k4-k1)/k1);
        sum4 += fabsf(100*(k5-k1)/k1);
        sum5 += fabsf(100*(k6-k1)/k1);
    }

    LOGD("atan2_approximation1: %.2f%%, atan2_approximation2: %.2f%%, atan2f_c: %.2f%%, fast_atan2: %.2f%%, fast_atan2_2: %.2f%%",
         sum1/nfft, sum2/nfft, sum3/nfft, sum4/nfft, sum5/nfft);

    LOGD("atan2f = %dus, atan2_approximation1 = %dus, atan2_approximation2 = %dus, atan2f_c= %dus, fast_atan2=%dus, fast_atan2_2=%dus",
         time2-time1, time3-time2, time4-time3, time5-time4, time6-time5, time7-time6);

//    for (int i = 0; i < nfft; i++) {
//        LOGD("%d: %.4f -> %.4f", f1[i], f6[i]);
//    }
};

