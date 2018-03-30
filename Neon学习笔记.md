 - 为了能调试Neon，查看变量值，编译时配置：

 首先给编译优化关掉
 ```
 # Android.mk 

 LOCAL_CFLAGS += -mfpu=neon -std=c++11

#要给优化关掉
#LOCAL_CFLAGS += -ffast-math -Os -funroll-loops
#LOCAL_CFLAGS += -fvisibility=hidden
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

