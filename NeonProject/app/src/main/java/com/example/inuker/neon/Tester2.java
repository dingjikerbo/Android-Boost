package com.example.inuker.neon;

import android.util.Log;

import java.util.Arrays;

public class Tester2 extends AbsTester {

    private byte[] A, B;
    private static final int COUNT = 1600000;

    @Override
    public void prepare() {
        A = new byte[COUNT / 4];

        B = new byte[COUNT];

        for (int i = 0; i < B.length; i++) {
            B[i] = 1;
        }
    }

    @Override
    public void test() {
        int c1 = 99, c2 = 137, c3 = 257, c4 = 981;

        Arrays.fill(A, (byte) 0);
        testC(A, B, COUNT, c1, c2, c3, c4);

        StringBuilder sb = new StringBuilder();
        for (byte b : A) {
            sb.append(b + " ");
        }
        Log.v(TAG, String.format(">>> %s", sb.toString()));

        Arrays.fill(A, (byte) 0);
        testNeon(A, B, COUNT, c1, c2, c3, c4);

        sb = new StringBuilder();
        for (byte b : A) {
            sb.append(b + " ");
        }
        Log.v(TAG, String.format(">>> %s", sb.toString()));
    }

    public native void testC(byte[] A, byte[] B, int len, int p1, int p2, int p3, int p4);

    public native void testNeon(byte[] A, byte[] B, int len, int p1, int p2, int p3, int p4);
}
