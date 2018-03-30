LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := native-lib

LOCAL_SRC_FILES  := helloneon.cpp utils.cpp

LOCAL_C_INCLUDES = $(LOCAL_PATH)/include

LOCAL_CFLAGS += -mfpu=neon -std=c++11
#LOCAL_CFLAGS += -ffast-math -Os -funroll-loops
#LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_LDLIBS := -llog
TARGET_ARCH_ABI := armeabi-v7a

LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)