LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LIBAV_PATH := $(LOCAL_PATH)/libav-12.3

LOCAL_MODULE := libav-fft

LOCAL_C_INCLUDES += $(LIBAV_PATH) \

LOCAL_SRC_FILES += $(LIBAV_PATH)/libavcodec/fft_fixed.c \
$(LIBAV_PATH)/libavcodec/fft_float.c \
$(LIBAV_PATH)/libavcodec/fft_template.c \
$(LIBAV_PATH)/libavcodec/arm/fft_fixed_init_arm.c \
$(LIBAV_PATH)/libavcodec/arm/fft_fixed_neon.S \
$(LIBAV_PATH)/libavcodec/arm/fft_init_arm.c \
$(LIBAV_PATH)/libavcodec/arm/fft_neon.S \
$(LIBAV_PATH)/libavcodec/arm/fft_vfp.S

LOCAL_CFLAGS += -mfpu=neon -Os
LOCAL_CFLAGS += -ffast-math -funroll-loops

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := libtestfft

LOCAL_SRC_FILES := main.cpp

LOCAL_C_INCLUDES :=

LOCAL_CFLAGS += -std=c++11 -fexceptions -frtti
LOCAL_CFLAGS += -mfpu=neon -Os
LOCAL_CFLAGS += -ffast-math -funroll-loops
LOCAL_LDLIBS += -llog

LOCAL_ARM_NEON := true
LOCAL_ARM_MODE := arm

LOCAL_SHARED_LIBRARIES := libav-fft

include $(BUILD_SHARED_LIBRARY)
