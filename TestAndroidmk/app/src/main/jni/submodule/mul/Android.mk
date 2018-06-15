LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(LOCAL_PATH)/mul.cpp

LOCAL_MODULE := mul

include $(BUILD_SHARED_LIBRARY)