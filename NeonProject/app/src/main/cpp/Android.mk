LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := native-lib

LOCAL_SRC_FILES  := utils.c \
    main.c


LOCAL_C_INCLUDES = $(LOCAL_PATH)/include

LOCAL_CFLAGS += -mfpu=neon
LOCAL_CFLAGS += -ffast-math -O3 -funroll-loops
LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_LDLIBS := -llog
#LOCAL_SHARED_LIBRARIES := libNE10

TARGET_ARCH_ABI := armeabi-v7a

LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

include $(BUILD_SHARED_LIBRARY)