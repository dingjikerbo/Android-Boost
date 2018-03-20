#include <jni.h>
#include <string>

#include <arm_neon.h>

int array1[20] = {
        2, 1, 4, 16
};

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_inuker_neon_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    int32x4_t a = vld1q_s32(array1);
    int16x4_t c = vqshrn_n_s32(a, 1);

    return env->NewStringUTF(hello.c_str());
}
