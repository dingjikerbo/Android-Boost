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
            A[i] = i;
        }
        new Thread() {
            @Override
            public void run() {
                mytest(A, count);
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
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void mytest(int[] A, int len);
}
