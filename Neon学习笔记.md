一、为了能调试Neon，查看变量值，编译时配置：

首先给编译优化关掉，

Android.mk
```
LOCAL_CFLAGS += -mfpu=neon -std=c++11

# 要给优化关掉
# LOCAL_CFLAGS += -ffast-math -Os -funroll-loops
# LOCAL_CFLAGS += -fvisibility=hidden
```

此外APP_OPTIM要设成debug

```
# Application.mk
APP_STL := gnustl_static
APP_ABI := armeabi-v7a
APP_PLATFORM := android-23
APP_CPPFLAGS += -std=c++11
APP_OPTIM := debug
```

二、反编译代码

- 给~/Library/Android/sdk/ndk-bundle/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-objdump加到PATH
- 编译时给代码优化打开，

```
arm-linux-androideabi-objdump -d libnative-lib.so
```


三、对于4通道的，int8x8_t，如何只存储前半部分
vst1最少要存到D寄存器，即64位，而int8x8_t前部半分只是32位，如何存储
vst1_lane_s8可以一个字节一个自己的存储，但是要耗费4条指令

更好的办法是vst1_lane_s32，其声明如下，
void vst1_lane_s32 (int32_t *, int32x2_t, const int)

```
int8_t A[8] = {
    4, 8, 3, 1, 5, 6, 7, 8
};

int8x8_t d = vld1_s8(A);

int8_t *B = (int8_t *) malloc(8);
vst1_lane_s32(B, d, 0);
```

通过这种方式，B的前4个字节分别是4, 8, 3, 1
即将B当成是int32_t的数组，将d当成是int32x2_t的向量，这样将32位的数据整体拷贝到数组中。





