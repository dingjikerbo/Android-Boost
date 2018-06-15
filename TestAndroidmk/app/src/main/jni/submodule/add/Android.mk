LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(LOCAL_PATH)/add.cpp

LOCAL_MODULE := add

include $(BUILD_SHARED_LIBRARY)