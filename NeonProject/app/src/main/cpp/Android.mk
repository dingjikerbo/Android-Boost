LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := native-lib

LOCAL_SRC_FILES  := utils.cpp \
    test0.cpp \
    test1.cpp \
    test2.cpp

LOCAL_C_INCLUDES = $(LOCAL_PATH)/include

LOCAL_CFLAGS += -mfpu=neon -std=c++11
LOCAL_CFLAGS += -ffast-math -Os -funroll-loops
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_LDLIBS := -llog
#LOCAL_SHARED_LIBRARIES := libNE10

TARGET_ARCH_ABI := armeabi-v7a

LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)