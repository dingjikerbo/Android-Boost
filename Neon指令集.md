# vld1q_s8
##### int8x16_t vld1q_s8 (const int8_t *) 
```
int8_t *bottom_base = new int8_t[20] {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 21, 22, 23,24
};
int8x16_t Q = vld1q_s8(bottom_base);

Q = {1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16}
```
> 从数组读16个sint8到8x16的向量中

# vld2_s16
##### int16x4x2_t vld2_s16 (int16_t const * ptr)
```
short A[8] = {
        1, 2, 3, 4, 5, 6, 7, 8
};
int16x4x2_t d = vld2_s16(A);
int16x4_t d0 = d.val[0];
int16x4_t d1 = d.val[1];
d0 = {1, 3, 5, 7}
d1 = {2, 4, 6, 8}
```
> 从数组中交叉读取sint16到两个16x4的向量中

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
> 表示给8x16的向量存到数组中

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
> vget_low_s8是取Q寄存器的低半部分，vget_high_s8是取Q寄存器的低半部分

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
> 是取Q寄存器向量中的第n个lane

# vdupq_n_s8/vmovq_n_s8
##### int8x16_t vdupq_n_s8 (int8_t) 
##### int8x16_t vmovq_n_s8 (int8_t) 
```
int8x16_t Q = vdupq_n_s8(3);
Q = {3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3}
```
> 用于生成一个向量存在Q寄存器中，向量中都用n填充，这两个函数功能一样。

# vaddq_s32
##### int32x4_t vaddq_s32 (int32x4_t, int32x4_t) 
```
int *bottom_base = new int[8] {
    1, 2, 3, 4, 5, 6, 7, 8
};
int32x4_t q1 = vld1q_s32(bottom_base);
int32x4_t q2 = vld1q_s32(bottom_base + 4);
int32x4_t q3 = vaddq_s32(q1, q2);

q3 = {6, 8, 10, 12}
```
> 返回的向量是两个向量之和

# vmlaq_n_s32(a, b, c)
##### int32x4_t vmlaq_n_s32 (int32x4_t, int32x4_t, int32_t)
```
int *bottom_base = new int[8] {
    1, 2, 3, 4, 5, 6, 7, 8
};
int32x4_t q1 = vld1q_s32(bottom_base);
int32x4_t q2 = vld1q_s32(bottom_base + 4);
int32x4_t q3 = vmlaq_n_s32(q1, q2, 3);

q3 = {16 20 24 28}
```
> 向量q2乘以一个系数，再加上向量q1

# vmovl_s8/vqmovn_s32
##### int16x8_t vmovl_s8 (int8x8_t) 
```
int8_t *p8 = new int8_t[8] {
    1, 2, 3, 4, 5, 6, 7, 8
};
int8x8_t d1 = vld1_s8(p8);
int16x8_t q1 = vmovl_s8(d1);

q1 = {1 2 3 4 5 6 7 8}
```
> 这里vmovl_s8意思是给s8扩充成s16，而vqmovn_s32是给s32缩为s16

# vqshlq_s32
##### int32x4_t vqshlq_s32 (int32x4_t, int32x4_t)
```
int *p32 = new int[8] {
    1, 2, 3, 4, 5, 6, 7, 8
};
int32x4_t q1 = vdupq_n_s32(1);
int32x4_t q2 = vld1q_s32(p32);
int32x4_t q3 = vqshlq_s32(q1, q2);

q3 = {2 4 8 16}
```
> q1向量往左移动，注意q2也是向量，当q1中是负数时意味着向右移动

# vsli_n_s8
##### int8x8_t vsli_n_s8 (int8x8_t, int8x8_t, const int)
```
int8_t *p8 = new int8_t[8] {
    1, 2, 3, 4, 5, 6, 7, 8
};
int8x8_t d1 = vld1_s8(p8);
int8x8_t d2 = vdup_n_s8(5);
int8x8_t d3 = vsli_n_s8(d1, d2, 2);

d3 = {21 22 23 20 21 22 23 20}
```
> 首先d2向量的元素左移2个bit位，然后空出来的bit位用d1向量对应元素的低2个bit位替换。

# vqshrn_n_s32
##### int16x4_t vqshrn_n_s32 (int32x4_t, const int)
```
int *p32 = new int[4] {
    4, 8, 12, 16
};
int32x4_t q1 = vld1q_s32(p32);
int16x4_t d1 = vqshrn_n_s32(q1, 2);

d1 = {1, 2, 3, 4}
```
> 这个右移和左移的区别在右移传入的是整数，而左移传入的是向量，支持每个lane的shift都不一样且要注意这里的shift不能超过16
另外这里是饱和，且输出相对输入被折半了，所谓饱和就是当值超出范围时被限定住

# vcombine_s16
##### int16x8_t vcombine_s16 (int16x4_t, int16x4_t)
```
int *p32 = new int[8] {
	1, 2, 3, 4, 5, 6, 7, 8
};
int32x2_t d1 = vld1_s32(p32);
int32x2_t d2 = vld1_s32(p32 + 4);
int32x4_t q1 = vcombine_s32(d1, d2);

q1 = {1 2 5 6}
```
> 给两个向量拼接成一个16x8的向量，这里p0在前，p1在后

# vpadd_s32
##### int32x2_t vpadd_s32 (int32x2_t a, int32x2_t b)
```
int *p32 = new int[8] {
    1, 2, 3, 4, 5, 6, 7, 8
};
int32x2_t d1 = vld1_s32(p32);
int32x2_t d2 = vld1_s32(p32 + 4);
int32x2_t d3 = vpadd_s32(d1, d2);

d3 = {3, 11}
``` 
> 即将d1向量的两个元素相加，d2向量的两个元素相加，两个结果拼接成新向量。

# vpadalq_s16
##### int32x4_t vpadalq_s16 (int32x4_t a, int16x8_t b)
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
> 先将b两两相加，得到一个16x4的向量，然后和a向量相加即可。所谓的b两两相加，即b[0]+b[1]，b[2]+b[3]，b[4]+b[5], b[6]+b[7]

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

# vqdmulh_s16
##### int16x4_t vqdmulh_s16 (int16x4_t a, int16x4_t b)
```
short A[16] = {
        100, 200, 300, 400, 500, 600, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};

int16x4x2_t d = vld2_s16(A);

int16x4_t d0 = d.val[0];
int16x4_t d1 = d.val[1];

int16x4_t D = vqdmulh_s16(d0, d1);
D = {0, 3, 9, 0}
```
> 将d0和d1相乘，结果再乘2，然后返回高一半位，或相当于结果>>16。因为此处两个16位向量相乘，结果应该为32位向量，所以高一半应该是16位。

# vreinterpretq/vreinterpret
##### int32x2_t vreinterpret_s32_u8 (uint8x8_t)
```
uint8x8_t q0 = {1, 2, 3, 4, 5, 6, 7, 8}
uint32x2_t q1 = vreinterpret_s32_u8(q0);
q1 = {0x04030201, 0x08070605}
```
这里是按内存排布转换的，不是完全数字意义上的转换。如果要15.3转成15就要用vcvt。

# vcvtq_s32_f32
##### int32x4_t vcvtq_s32_f32 (float32x4_t a)
```
float num[] = {
        1.0, 1.2, 1.5, 1.8
};

float32x4_t ff = vld1q_f32(num);
int32x4_t qs = vcvtq_s32_f32(ff);
qs = {1, 1, 1, 1} 
```
> 对于正数，这里直接向下取整，如果都改成负数，则qs为-1, -1, -1, -1，如果想真正的向下取整怎么办

# vrev64q_s16
##### int16x4_t vrev64_s16 (int16x4_t vec)
```
short A[16] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};

int16x8_t q = vld1q_s16(A);
q = vrev64q_s16(q);
q = {4, 3, 2, 1, 8, 7, 6, 5}
```
> 向量的每个元素倒序

# vext_s8
##### int8x8_t vext_s8 (int8x8_t a, int8x8_t b, const int n)
```
int8_t A[] ={
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};
int8x8_t a = vld1_s8(A);
int8x8_t b = vld1_s8(A + 2);
int8x8_t c = vext_s8(a, b, 1);
c = {2 3 4 5 6 7 8 3}
```
> 去掉a的头1个，取a剩下的元素再拼上b的头1个元素

# vld1_lane_s8
##### uint8x8_t vld1_lane_u8 (uint8_t const * ptr, uint8x8_t src, const int lane)
```
int8_t a[] = {
        1, 2, 3, 4, 5, 6, 7, 8
};
int8_t b[] = {
        11, 12, 13, 14, 15, 16, 17, 18
};
int8_t *p = b;
int8x8_t c = vld1_s8(a);
int8x8_t d = vld1_lane_s8(p, c, 5);
d = {1, 2, 3, 4, 5, 11, 7, 8}
```
> 即先给c拷贝到d，然后将d中的第5个lane替换成p指向的值。

# vst1_lane_s8
##### void vst1_lane_s8 (int8_t * ptr, int8x8_t val, const int lane)
```
int8_t a[] = {
        1, 2, 3, 4, 5, 6, 7, 8
};
int8_t b[] = {
        11, 12, 13, 14, 15, 16, 17, 18
};
int8_t *p = b;
int8x8_t c = vld1_s8(a);
vst1_lane_s8 (b, c, 1);
b = {2, 12, 13, 14, 15, 16, 17}
```
> 给c向量的第1个lane存到b指向的内存

# vtbl1_s8
##### int8x8_t vtbl1_s8 (int8x8_t a, int8x8_t b)
```
int8_t a[] = {
        1, 2, 3, 4, 5, 6, 7, 8
};
int8_t b[] = {
        0, 1, 1, 2, 0, 1, 1, 0
};
int8_t *p = b;
int8x8_t c = vld1_s8(a);
int8x8_t d = vld1_s8(b);
int8x8_t e = vtbl1_s8(c, d);
e = {1 2 2 3 1 2 2 1}
```
> 以d向量中的元素为索引，从c向量中取对应的元素，最后组成一个向量。tbl只能用于int8

# vtbl2_s8
##### int8x8_t vtbl2_s8 (int8x8x2_t a, int8x8_t b)
```
int8_t a[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
int8_t b[] = {
        0, 9, 1, 2, 0, 1, 1, 0
};
int8x8x2_t c = vld2_s8(a);
int8x8_t d = vld1_s8(b);
int8x8_t e = vtbl2_s8(c, d);
e = {1 4 3 5 1 3 3 1}
```
> 这个和上个的区别在于这里多了一个维度，所以可以给c向量看成两行，d中的索引当大于1行时就认为是从第二行的向量取元素。tbl只能用于int8

# vtrn_s8
##### int8x8x2_t vtrn_s8 (int8x8_t a, int8x8_t b)
```
int8_t a[] = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
};
int8_t b[] = {
        0, 9, 1, 2, 0, 1, 1, 0
};
int8x8_t d1 = vld1_s8(a);
int8x8_t d2 = vld1_s8(b);
int8x8x2_t d3 = vtrn_s8(d1, d2);
d3.val[0] = {1, 0, 3, 1, 5, 0, 7, 1}
d3.val[1] = {2, 9, 4, 2, 6, 1, 8, 0}
```
> 将d1和d2两个向量交叉取值

# vcleq_s32/vcgeq_s32
##### uint32x4_t vcleq_s32 (int32x4_t a, int32x4_t b)
```
int a[] = {
        100, 2, 3, 400, 5, 6, 7, 8
};
int b[] = {
        10, 20, 30, 40, 50, 60, 70, 80
};
int32x4_t aa = vld1q_s32(a);
int32x4_t bb = vld1q_s32(b);
uint32x4_t cc = vcleq_s32(aa, bb);
uint32x4_t dd = vcgeq_s32(aa, bb);
cc = {0, 0xffffffff, 0xffffffff, 0}
dd = {0xffffffff, 0, 0, 0xffffffff}
```
> vcleq_s32表示检测aa是否小于bb，如果小于则对应的向量所有bit位全部置为1，否则全部置为0
> vcgeq_s32检测aa是否大于等于bb

# vbslq_s32
##### float32x4_t vbslq_f32 (uint32x4_t a, float32x4_t b, float32x4_t c)
```
int a[] = {
        0x10, 0x101, 3, 400, 5, 6, 7, 8
};
int b[] = {
        0x111, 0x10, 30, 40, 50, 60, 70, 80
};
int32x4_t aa = vld1q_s32(a);
int32x4_t bb = vld1q_s32(b);
int32x4_t cc = vcgeq_s32(aa, bb);

int32x4_t uu = vdupq_n_s32(-1);

uint32x4_t ff = vbslq_s32(cc, aa, uu);
ff = {-1, 257, -1, 400}
```
> 表示如果cc的向量是bit位全1，则取aa的对应向量，否则取uu的对应向量。

# vrsqrteq_f32
##### float32x4_t vrsqrteq_f32 (float32x4_t a)
##### uint32x4_t vrsqrteq_u32 (uint32x4_t a)
##### float32x2_t vrsqrte_f32 (float32x2_t a)
```
float num[] = {
        1.44, 2.25, 1.21, 1.69
};
float32x4_t qf = vld1q_f32(num);
qf = vrsqrteq_f32(qf);
qf = {0.832031 0.666016 0.910156 0.769531}
```
> 这个函数返回向量sqrt的倒数，可见精度大概是小数点后两位，vsqrt只能用于A64.

# vrsqrtsq_f32
##### float32x4_t vrsqrtsq_f32 (float32x4_t a, float32x4_t b)
```
float arr[] = {
        1.0f, 4.0f, 9.0f, 16.0f
};
float32x4_t qs = vld1q_f32(arr);
qs = vrsqrtsq_f32(qs, vdupq_n_f32(4.0));
qs = {-0.5 -6.5 -16.5 -30.5}
```
> a每一个元素乘以b每一个元素，然后3.0减去每个结果，除以2

# vzipq_f32
##### float32x4x2_t vzipq_f32 (float32x4_t a, float32x4_t b)

```
float n1[] = {
        1., 2., 3., 4.
};
float n2[] = {
        5., 6., 7., 8.
};
float32x4_t qf1 = vld1q_f32(n1);
float32x4_t qf2 = vld1q_f32(n2);
float32x4x2_t qf = vzipq_f32(qf1, qf2);
qf.val[0] = {1, 5, 2, 6}
qf.val[1] = {3, 7, 4, 8}
```

# vrecpeq_f32
##### float32x4_t vrecpeq_f32 (float32x4_t a)
```
float arr[] = {
        1.0f, 4.0f, 9.0f, 16.0f
};
float32x4_t qs = vld1q_f32(arr);
qs = vrecpeq_f32(qs);
qs = {1.0, 0.25, 0.11, 0.06}
```
> 这个函数返回每个元素的倒数

# vrecpsq_f32
##### float32x4_t vrecpsq_f32 (float32x4_t a, float32x4_t b)
```
float arr[] = {
        1.0f, 4.0f, 9.0f, 16.0f
};
float32x4_t qs = vld1q_f32(arr);
qs = vrecpsq_f32(qs, vdupq_n_f32(2.0));
qs = {0, -6, -16, -30}
```
> a每个元素乘以b的每个元素，然后2.0减去每个结果

# vcageq_f32
##### uint32x4_t vcageq_f32 (float32x4_t a, float32x4_t b)
```
float n1[] = {
        1.0f, 2.0f, 3.0f, 4.0f
};

float n2[] ={
    -2.0f, 1.0f, -3.0f, 5.0f
};

float32x4_t qf1 = vld1q_f32(n1);
float32x4_t qf2 = vld1q_f32(n2);
uint32x4_t q1 = vcageq_f32(qf1, qf2);
q1 = {0, -1, -1, 0}
```
> 比较qf1和qf2的绝对值大小，如果大于等于则为全1，否则为0
> 类似的有vcagtq_f32和vcaltq和vcaleq等

# vmaxq_f32/vminq_f32
##### float32x4_t vmaxq_f32 (float32x4_t a, float32x4_t b)
##### float32x4_t vminq_f32 (float32x4_t a, float32x4_t b)
```
float n1[] = {
        1.0f, 2.0f, 3.0f, 4.0f
};

float n2[] ={
    -2.0f, 1.0f, -3.0f, 5.0f
};

float32x4_t qf1 = vld1q_f32(n1);
float32x4_t qf2 = vld1q_f32(n2);
float32x4_t qf3 = vmaxq_f32(qf1, qf2);
qf3 = {1.0, 2.0, 3.0, 5.0}
```
> 这里是返回最大值

# vabsq_f32
##### float32x4_t vabsq_f32 (float32x4_t a)
```
float n2[] ={
    -2.0f, 1.0f, -3.0f, 5.0f
};

float32x4_t qf2 = vld1q_f32(n2);
float32x4_t qf3 = vabsq_f32(qf2);
qf3 = {2.0, 1.0, 3.0, 5.0}
```
> 返回绝对值

# vnegq_f32
##### float32x4_t vnegq_f32 (float32x4_t a)
```
float n2[] ={
    -2.0f, 1.0f, -3.0f, 5.0f
};
float32x4_t qf2 = vld1q_f32(n2);
float32x4_t qf3 = vnegq_f32(qf2);
qf3 = {2.0, -1.0, 3.0f, -5.0}
```
> 返回负乘数

# vabaq_s32
##### int32x4_t vabaq_s32 (int32x4_t a, int32x4_t b, int32x4_t c)
```
int num1[] = {
        1, 2, 3, 4
};
int num2[] = {
        7, 6, 4, 1
};
int32x4_t q1 = vld1q_s32(num1);
int32x4_t q2 = vld1q_s32(num2);
int32x4_t q3 = vdupq_n_s32(1);
q3 = vabaq_s32(q3, q1, q2);
q3 = {7, 5, 2, 4}
```
> 返回q1和q2的差的绝对值，加上q3