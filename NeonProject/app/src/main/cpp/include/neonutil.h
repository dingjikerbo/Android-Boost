//
// Created by 李文田 on 2018/7/23.
//

#ifndef NEONPROJECT_NEONUTIL_H
#define NEONPROJECT_NEONUTIL_H

#include <jni.h>
#include <arm_neon.h>

extern void show8(int8x8_t d);
extern void show8q(int8x16_t q);
extern void show16(int16x4_t q);
extern void show16q(int16x8_t q);
extern void show32q(int32x4_t q);
extern void show32(int32x2_t d);
extern void showu32q(uint32x4_t q);
extern void show32fq(float32x4_t qf);

#endif //NEONPROJECT_NEONUTIL_H
