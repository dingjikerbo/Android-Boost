#include <jni.h>
#include <stdlib.h>
#include <string.h>

static void
dot_mul_one_line(int8_t *pA, int8_t *pB, int8_t *pC, int32_t len) {
    for (int32_t i = 0; i < len; i++) {
        pC[i] += pA[i] * pB[i];
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_inuker_testdilationconv_MainActivity_testDilationConv(JNIEnv *env, jobject thiz,
        jbyteArray input, jbyteArray weight, jbyteArray output, int inputSize, int channels, int kernelSize, int dilation, int stride, int padding) {

    int8_t *pInput = env->GetByteArrayElements(input, NULL);
    int8_t *pWeight = env->GetByteArrayElements(weight, NULL);
    int8_t *pOutput = env->GetByteArrayElements(output, NULL);

    int dilationKernelSize = (kernelSize - 1) * dilation + 1;
    int outputSize = (inputSize + 2 * padding - dilationKernelSize) / stride + 1;

    int h_pad = -padding;

    for (int h = 0; h < outputSize; ++h) {

        int w_pad = -padding;

        for (int w = 0; w < outputSize; ++w) {
            int weight_index = 0;

            int8_t *top_temp_data = pOutput + (h * outputSize + w) * channels;
            memset(top_temp_data, 0, sizeof(int8_t) * channels);

            for (int32_t ih = h_pad; ih < h_pad + dilationKernelSize; ih += dilation) {
                for (int32_t iw = w_pad; iw < w_pad + dilationKernelSize; iw += dilation) {
                    if (ih >= 0 && ih < inputSize && iw >= 0 && iw < inputSize) {
                        int8_t *bottom_temp_data = pInput + (ih * inputSize + iw) * channels;
                        int8_t *weight_temp_data = pWeight + weight_index * channels;
                        dot_mul_one_line(bottom_temp_data, weight_temp_data, top_temp_data, channels);
                    }
                    weight_index++;
                }
            }
            w_pad += stride;
        }
        h_pad += stride;
    }

    env->ReleaseByteArrayElements(input, pInput, 0);
    env->ReleaseByteArrayElements(weight, pWeight, 0);
    env->ReleaseByteArrayElements(output, pOutput, 0);
}
