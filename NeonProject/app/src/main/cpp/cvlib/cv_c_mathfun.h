/*
 * Implementation approximate Math library in Neon and single float.
 * Exp Log Sin Cos is almost the same Julien Pommier's NEON implementation.
 * Some of the algorithms not is based on the cephes math library.
 *
 * liuyang.10 2018.06.24
*/

#ifndef __CV_LIBRARY_C_MATHFUN_H__
#define __CV_LIBRARY_C_MATHFUN_H__

#include <math.h>
#include "cv_math_define.h"

namespace cv_maths
{
float logf_single(float xx);

float exp_single(float x);

float sinf_single(float xx);

float cosf_single(float xx);

float tancotf_single(float xx,int cotflg);

float tanf_single(float x);

float cotf_single(float x);

float atanf_single(float xx);

float atan2f_single(float y,float x);

float exp_remez_single(float x);

float inv_sqrt_single(float xx);

float q_rsqrt_single(float number);


}
#endif
