/*
 * Implementation approximate Math library in Neon and single float.
 * Exp Log Sin Cos is almost the same Julien Pommier's NEON implementation.
 * Some of the algorithms not is based on the cephes math library.
 *
 * liuyang.10 2018.06.24
*/

#ifndef __CV_LIBRARY_NEON_MATHFUN_H__
#define __CV_LIBRARY_NEON_MATHFUN_H__

namespace cv_maths
{
#ifdef __ARM_NEON__
#include <arm_neon.h>
/* natural logarithm computed for 4 simultaneous float 
   return NaN for x <= 0
*/
float32x4_t log_ps(float32x4_t x);

/* exp() computed for 4 float at once */
float32x4_t exp_ps(float32x4_t x);

/* evaluation of 4 sines & cosines at once.

   The code is the exact rewriting of the cephes sinf function.
   Precision is excellent as long as x < 8192 (I did not bother to
   take into account the special handling they have for greater values
   -- it does not return garbage for arguments over 8192, though, but
   the extra precision is missing).

   Note that it is such that sinf((float)M_PI) = 8.74e-8, which is the
   surprising but correct result.

   Note also that when you compute sin(x), cos(x) is available at
   almost no extra price so both sin_ps and cos_ps make use of
   sincos_ps..
  */
void sincos_ps(float32x4_t x,float32x4_t *ysin,float32x4_t *ycos);

float32x4_t sin_ps(float32x4_t x);

float32x4_t cos_ps(float32x4_t x);


float32x4_t tanf_cephes(float32x4_t x );

float32x4_t cotf_cephes(float32x4_t x );

float32x4_t exp_remez(float32x4_t x);

float32x4_t inv_sqrt_float32x4( float32x4_t xx );

float32x4_t q_rsqrt_float32x4( float32x4_t xx);

float32x4_t atanf_cephes(float32x4_t xx);

float32x4_t atan2f_cephes(float32x4_t y,float32x4_t x);
}
#endif // __ARM_NEON__

#endif // __NEON_MATHFUN_H__

