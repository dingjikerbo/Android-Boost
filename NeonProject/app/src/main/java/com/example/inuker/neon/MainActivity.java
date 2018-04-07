package com.example.inuker.neon;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

    private Button mBtnTest;
    private volatile boolean mTesting;

    private Tester mTester = new Tester0();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mBtnTest = findViewById(R.id.test);
        mBtnTest.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                if (!mTesting) {
                    mTesting = true;
                    mBtnTest.setText("TESTING");
                    startTest();

                }
            }
        });
    }

    private void startTest() {
        mTester.prepare();

        new Thread() {
            @Override
            public void run() {
                mTester.test();

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

}
