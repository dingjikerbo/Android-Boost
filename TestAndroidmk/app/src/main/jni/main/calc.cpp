//
// Created by 李文田 on 2018/6/15.
//
#include <jni.h>
#include <string>

#include <android/log.h>
#define LOG_TAG "bush"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

int add(int a, int b);
int mul(int a, int b);


int calc(int a, int b) {
    int k1 = add(a, 1);
    int k2 = add(b, 2);
    return mul(k1, k2);
}

int main() {
    int a = 2, b = 3;
    printf("a = %d", a);
    printf("b = %d", b);
    printf("calc = %d", calc(a, b));
    return 0;
}

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_example_inuker_testandroidmk_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    int res = calc(5, 8);
    LOGD("res = %d", res);
    return env->NewStringUTF(hello.c_str());
}
