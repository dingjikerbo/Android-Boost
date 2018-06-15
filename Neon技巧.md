1, neon改类型，
```
uint8x8_t 
dA = A0 A1 A2 A3 A4 A5 A6 A7
dB = B0 B1 B2 B3 B4 B5 B6 B7
dC = C0 C1 C2 C3 C4 C5 C6 C7
dD = D0 D1 D2 D3 D4 D5 D6 D7
dE = E0 E1 E2 E3 E4 E5 E6 E7
dF = F0 F1 F2 F3 F4 F5 F6 F7
dG = G0 G1 G2 G3 G4 G5 G6 G7
dH = H0 H1 H2 H3 H4 H5 H6 H7

如何生成如下的内存布局
A0, B0, C0, D0, E0, F0, G0, H0, 
A1, B1, C1, D1, E1, F1, G1, H1,
A2, B2, C2, D2, E2, F2, G2, H2, 
A3, B3, C3, D3, E3, F3, G3, H3,
A4, B4, C4, D4, E4, F4, G4, H4, 
A5, B5, C5, D5, E5, F5, G5, H5,
A6, B6, C6, D6, E6, F6, G6, H6, 
A7, B7, C7, D7, E7, F7, G7, H7
...

首先vtrn_u8(dA, dB)，生成uint8x8x2_t
qAB = 
A0 B0 A2 B2 A4 B4 A6 B6
A1 B1 A3 B3 A5 B5 A7 B7

同理依次处理C, D, E, F, G, H，如下
qCD = 
C0 D0 C2 D2 C4 D4 C6 D6
C1 D1 C3 D3 C5 D5 C7 D7

qEF = 
E0 F0 E2 F2 E4 F4 E6 F6
E1 F1 E3 F3 E5 F5 E7 F7

qGH = 
G0 H0 G2 H2 G4 H4 G6 H6
G1 H1 G3 H3 G5 H5 G7 H7

然后给uint8x8x2_t的向量combine，得到uint8x16_t，如下：
qcAB = 
A0 B0 A2 B2 A4 B4 A6 B6 A1 B1 A3 B3 A5 B5 A7 B7 

同理
qcCD = 
C0 D0 C2 D2 C4 D4 C6 D6 C1 D1 C3 D3 C5 D5 C7 D7

qcEF = 
E0 F0 E2 F2 E4 F4 E6 F6 E1 F1 E3 F3 E5 F5 E7 F7

qcGH = 
G0 H0 G2 H2 G4 H4 G6 H6 G1 H1 G3 H3 G5 H5 G7 H7

再vtrn_u16(qcAB, qcCD)，得到uint16x8x2_t如下

uint16x8x2_t qABCD = 
A0 B0 C0 D0 A4 B4 C4 D4 A1 B1 C1 D1 A5 B5 C5 D5 
A2 B2 C2 D2 A6 B6 C6 D6 A3 B3 C3 D3 A7 B7 C7 D7

同理
uint16x8x2_t qEFGH = 
E0 F0 G0 H0 E4 F4 G4 H4 E1 F1 G1 H1 E5 F5 G5 H5
E2 F2 G2 H2 E6 F6 G6 H6 E3 F3 G3 H3 E7 F7 G7 H7

再vtrn_u32(qABCD.val[0], qEFGH.val[0])得到uint32x4x2_t如下:
uint32x4_t qq1 = 
A0 B0 C0 D0 E0 F0 G0 H0 A1 B1 C1 D1 E1 F1 G1 H1
A4 B4 C4 D4 E4 F4 G4 H4 A5 B5 C5 D5 E5 F5 G5 H5

uint32x4_t qq2 = 
A2 B2 C2 D2 E2 F2 G2 H2 A3 B3 C3 D3 E3 F3 G3 H3
A6 B6 C6 D6 E6 F6 G6 H6 A7 B7 C7 D7 E7 F7 G7 H7

再分别存储qq1.val[0]，qq2.val[0]，qq1.val[1]，qq2.val[1]，如下：
A0 B0 C0 D0 E0 F0 G0 H0 A1 B1 C1 D1 E1 F1 G1 H1
A2 B2 C2 D2 E2 F2 G2 H2 A3 B3 C3 D3 E3 F3 G3 H3
A4 B4 C4 D4 E4 F4 G4 H4 A5 B5 C5 D5 E5 F5 G5 H5
A6 B6 C6 D6 E6 F6 G6 H6 A7 B7 C7 D7 E7 F7 G7 H7
```

2, vrev颠倒的



3, 查表法
uint8x8_t 内部调整顺序的


4, int16x8_t q = [K0 K0 K2 K2 K4 K4 K6 K6]
输出d = [K0 K2 K4 K6]
vmovn_s32(q)即可

5, 对于3通道的alpha，如下K0K1 K0K1 K0K1 K2K3 K2K3 K2K3这样的，每个像素都对应两个short，一个通道3个像素，这3个像素的k都是一样的。
如果要取两个向量分别是{K0 K2 K0 K2}和{K1 K3 K1 K3}，只要加载alpha时往后偏一个像素，

```
int16x8_t dalpha = vld1q_s16(alpha + dx * 2 + 2);// K0 K1 K0 K1 K2 K3 K2 K3
int16x4x2_t das = vtrn_s16(vget_low_s16(dalpha), vget_high_s16(dalpha));
int16x4_t da0 = das.val[0];// K0 K2 K0 K2
int16x4_t da1 = das.val[1];// K1 K3 K1 K3
```

当然也可以查表
```
uint8_t offset2[] = {
        0, 1, 4, 5, 2, 3, 6, 7
};
uint8x8_t doff2 = vld1_u8(offset2);

uint16x4_t dt0 = vtbl1_u8(dalpha, doff2);// K0 K2 K1 K3
uint16x4_t dt1 = vrev64_s32(dt0); // K1 K3 K0 K2
uint16x4_t da0 = vext_u16(dt1, dt0, 2);// K0 K2 K0 K2
uint16x4_t da1 = vext_u16(dt0, dt1, 2);// K1 K3 K1 K3
```

6， alpha的布局是K0K1 K0K1 K0K1 K2K3 K2K3 K2K3 K4K5 K4K5 K4K5 K6K7 K6K7 K6K7
怎么样得到两个向量分别是{K0 K2 K4 K6}{K1 K3 K5 K7}

```
int16x8_t qalpha = vld3q_s16(alpha + dx * 2).val[0];// K0 K1 K2 K3 K4 K5 K6 K7

// K0 K0 K2 K2 K4 K4 K6 K6
// K1 K1 K3 K3 K5 K5 K7 K7
int16x8x2_t qtalpha = vtrnq_s16(qalpha, qalpha);
int16x4_t da0 = vmovn_s32(qtalpha.val[0]); // K0 K2 K4 K6
int16x4_t da1 = vmovn_s32(qtalpha.val[1]); // K1 K3 K5 K7
```
