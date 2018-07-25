LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cvmath
LOCAL_SRC_FILES := $(LOCAL_PATH)/cvlib/libcv_maths.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := native-lib

LOCAL_SRC_FILES  := utils.cpp \
    main.cpp \
    test_sincos.cpp


LOCAL_C_INCLUDES = $(LOCAL_PATH)/include \
$(LOCAL_PATH)/cvlib

LOCAL_CFLAGS += -mfpu=neon
LOCAL_CFLAGS += -ffast-math -O3 -funroll-loops
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_LDLIBS := -llog
LOCAL_STATIC_LIBRARIES := cvmath

TARGET_ARCH_ABI := armeabi-v7a

LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)