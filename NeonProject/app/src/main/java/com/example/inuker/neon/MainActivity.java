package com.example.inuker.neon;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

    private Button mBtnTest;
    private volatile boolean mTesting;

    private TextView mTvDesc;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mTvDesc = findViewById(R.id.text);

        mBtnTest = findViewById(R.id.test);
        mBtnTest.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                if (!mTesting) {
                    mTesting = true;
                    mBtnTest.setText("TESTING");
                    mTvDesc.setText("");
                    startTest();

                }
            }
        });
    }

    private void startTest() {
        final int count = 1000000;
        final int[] A = new int[count];
        for (int i = 0; i < A.length; i++) {
            A[i] = (byte) (i & 0xff);
        }
        final int[] B = new int[count];
        for (int i = 0; i < B.length; i++) {
            B[i] = (byte) (i & 0xf);
        }
        new Thread() {
            @Override
            public void run() {
                testNeon(A, B, count);
                testInstruction();
                mTesting = false;
                mBtnTest.post(new Runnable() {
                    @Override
                    public void run() {
                        mBtnTest.setText("TEST");
                    }
                });
            }
        }.start();

    }

    /**
     * 测试c和neon，对比性能
     */
    public native void testNeon(int[] A, int[] B, int len);

    /**
     * 测试指令
     */
    public native void testInstruction();
}
