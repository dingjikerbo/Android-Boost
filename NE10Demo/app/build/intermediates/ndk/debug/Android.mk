LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := NE10_test_demo
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_SRC_FILES := \
	/Users/dingjikerbo/dingjikerbo/Android-Boost/NE10Demo/app/src/main/jni/Android.mk \
	/Users/dingjikerbo/dingjikerbo/Android-Boost/NE10Demo/app/src/main/jni/CMakeLists.txt \
	/Users/dingjikerbo/dingjikerbo/Android-Boost/NE10Demo/app/src/main/jni/NE10_test_demo.c \

LOCAL_C_INCLUDES += /Users/dingjikerbo/dingjikerbo/Android-Boost/NE10Demo/app/src/main/jni
LOCAL_C_INCLUDES += /Users/dingjikerbo/dingjikerbo/Android-Boost/NE10Demo/app/src/debug/jni

include $(BUILD_SHARED_LIBRARY)
