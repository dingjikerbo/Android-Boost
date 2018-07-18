LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := app
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_SRC_FILES := \
	/Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni/com_hekai_fftw_android_Utils.c \
	/Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni/Android.mk \
	/Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni/fftw3/Android.mk \
	/Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni/fftw3/lib/libfftw3.a \
	/Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni/fftw3/lib/libfftw3f.a \
	/Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni/fftw3/lib/libfftw3f_threads.a \

LOCAL_C_INCLUDES += /Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/main/jni
LOCAL_C_INCLUDES += /Users/liwentian/dingjikerbo/Android-Boost/fftw_android/app/src/debug/jni

include $(BUILD_SHARED_LIBRARY)
