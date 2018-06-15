LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := calc.cpp
LOCAL_MODULE := mylib
LOCAL_SHARED_LIBRARIES := add mul
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)