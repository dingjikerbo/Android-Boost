package com.example.inuker.testdilationconv;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends Activity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        int inputSize = 4;
        int channels = 8;
        int kernelSize = 3;
        int dilation = 2;
        int stride = 1;
        int padding = 2;

        byte[] input = new byte[inputSize * inputSize * channels];
        for (int i = 0; i < input.length; i++) {
            input[i] = (byte) (i / channels);
        }

        byte[] weight = new byte[kernelSize * kernelSize * channels];
        for (int i = 0; i < weight.length; i++) {
            weight[i] = (byte) (i / channels);
        }

        int dilationKernelSize = (kernelSize - 1) * dilation + 1;
        int outputSize = (inputSize + 2 * padding - dilationKernelSize) / stride + 1;
        byte[] output = new byte[outputSize * outputSize * channels];

        testDilationConv(input, weight, output, inputSize, channels, kernelSize, dilation, stride, padding);

        for (int i = 0; i < output.length; i++) {
            Log.v("bush", String.format("%d ", output[i]));
            if (i > 0 && i % channels == 0) {
                Log.v("bush", "");
            }
        }
    }

    public native void testDilationConv(byte[] input, byte[] weight, byte[] output, int inputSize,
                                        int channels, int kernelSize, int dilation, int stride, int padding);
}
