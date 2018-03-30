# vld1q_s8
##### int8x16_t vld1q_s8 (const int8_t *) 
```
int8_t *bottom_base = new int8_t[20] {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 21, 22, 23,24
};
int8x16_t Q = vld1q_s8(bottom_base);

Q = {1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16}
```
从数组读16个sint8到8x16的向量中

# vst1q_s8
##### void vst1q_s8 (int8_t *, int8x16_t)
```
int8_t *bottom_base = new int8_t[16] {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
int8x16_t Q = vld1q_s8(bottom_base);
int8_t *out = new int8_t[16];
vst1q_s8(out, Q);

out = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}
```
表示给8x16的向量存到数组中

# vget_low_s8/vget_high_s8
##### int8x8_t vget_low_s8 (int8x16_t) 
##### int8x8_t vget_high_s8 (int8x16_t)
```
int8_t *bottom_base = new int8_t[16] {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
int8x16_t Q = vld1q_s8(bottom_base);
int8x8_t D0 = vget_low_s8(Q);
int8x8_t D1 = vget_high_s8(Q);

D0 = {1 2 3 4 5 6 7 8}
D1 = {9 10 11 12 13 14 15 16}
```
vget_low_s8是取Q寄存器的低半部分，vget_high_s8是取Q寄存器的低半部分

# vgetq_lane_s8(low, 0)
##### int8_t vgetq_lane_s8 (int8x16_t, const int)
```
int8_t *bottom_base = new int8_t[16] {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
int8x16_t Q = vld1q_s8(bottom_base);
int a = vgetq_lane_s8(Q, 3);

a = 4
```
是取Q寄存器向量中的第n个lane

# vdupq_n_s8/vmovq_n_s8
##### int8x16_t vdupq_n_s8 (int8_t) 
##### int8x16_t vmovq_n_s8 (int8_t) 
```
int8x16_t Q = vdupq_n_s8(3);
Q = {3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3}
```
用于生成一个向量存在Q寄存器中，向量中都用n填充，这两个函数功能一样。

# vaddq_u32
int8x16_t q_bb = vaddq_u8(q_b1, q_b2);
此处的q_b1和q_b2都是int8x16_t的，返回的向量是两个向量之和

# vmlaq_n_s32(a, b, c)
int32x4_t a = vld1q_s32(array1);
int32x4_t b = vld1q_s32(array1 + 2);
int32_t c = 10;
int32x4_t d = vmlaq_n_s32(a, b, c);
b向量乘以c，然后加上a向量，这里c只是个普通整数，相当于b向量中每个数都乘以c

# vmovl_s8
int8x16_t a = vld1q_s8(array0);
int16x8_t b = vmovl_s8(vget_low_s8(a));
int16x8_t c = vmovl_s8(vget_high_s8(a));
这里vmovl_s8意思是给s8扩充成s16，上面代码vget_low_s8(a)返回了8x8的向量，现在映射到了16x8的向量了

# vqshlq_s32
int32x4_t vqshlq_s32(int32x4_t __p0, int32x4_t __p1)
p0向量往左移动，注意p1也是向量，当p1中是负数时意味着向右移动

# vsli_n_s8
int8x8_t vsli_n_s8 (int8x8_t a, int8x8_t b, const int n)
这个函数坑很多，首先b向量左移n个bit位，然后空出来的bit位用a的低n个bit位替换。
int8x8_t a = vld1_s8(array0);
int8x8_t b = vdup_n_s8(5);
int8x8_t c = vsli_n_s8(a, b, 2);
假如a的第一个数是10，则此处先5 << 2为10100，12的二进制为1010，所以最后结果为10110，即22.

# vqshrn_n_s32
int16x4_t vqshrn_n_s32 (int32x4_t a, const int n)
int32x4_t a = vld1q_s32(array1);
int16x4_t c = vqshrn_n_s32(a, 2);
这个右移和左移的区别在右移传入的是整数，而左移传入的是向量，支持每个lane的shift都不一样
且要注意这里的shift不能超过16
另外这里是饱和，且输出相对输入被折半了，所谓饱和就是当值超出范围时被限定住

# vqmovn_s32
这个带后缀n表示结果会narrow了，之前是s32，结果是s16
int32x4_t a = vld1q_s32(array1);
int16x4_t b = vqmovn_s32(a);

# vcombine_s16
int16x8_t vcombine_s16(int16x4_t __p0, int16x4_t __p1)
表示给两个16x4的向量拼接成一个16x8的向量，这里p0在前，p1在后

# vpadd_s32
##### int32x2_t vpadd_s32 (int32x2_t a, int32x2_t b)
> 即将a向量的两个元素相加，b向量的两个元素相加，两个结果生成新向量。
```
int A[16] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
int32x2_t D = vpadd_s32(vld1_s32(A), vld1_s32(A + 2));
D = {3, 7}
``` 

# vpadalq_s16
##### int32x4_t vpadalq_s16 (int32x4_t a, int16x8_t b)
 > 先将b两两相加，得到一个16x4的向量，然后和a向量相加即可。所谓的b两两相加，即b[0]+b[1]，b[2]+b[3]，b[4]+b[5], b[6]+b[7]
```
int A[16] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
short B[8] = {
        1, 2, 3, 4, 5, 6, 7, 8
};
int32x4_t Q = vpadalq_s16(vld1q_s32(A), vld1q_s16(B));
Q = {4, 9, 14, 19}
```

# vmul/vmull
##### uint32x4_t vmull_u16 (uint16x4_t a, uint16x4_t b)
```
uint8_t A[16] = {
    1, 2, 3, 4, 5, 6, 7, 8, 
    9, 10, 11, 12, 13, 14, 15, 16
};
int16x8_t D = vmull_u8(vld1_u8(A), vld1_u8(A + 8));
D = {9 20 33 48 65 84 105 128}
```

# vreinterpretq/vreinterpret
##### int32x2_t vreinterpret_s32_u8 (uint8x8_t)
```
uint8x8_t q0 = {1, 2, 3, 4, 5, 6, 7, 8}
uint32x2_t q1 = vreinterpret_s32_u8(q0);
q1 = {0x04030201, 0x08070605}
```
