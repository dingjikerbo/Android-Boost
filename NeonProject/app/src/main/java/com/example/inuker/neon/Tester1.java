package com.example.inuker.neon;

public class Tester1 extends AbsTester {

    private int[] A, B;
    private static final int COUNT = 1000000;

    @Override
    public void prepare() {
        A = new int[COUNT];
        for (int i = 0; i < A.length; i++) {
            A[i] = (byte) (i & 0xff);
        }
        B = new int[COUNT];

        for (int i = 0; i < B.length; i++) {
            B[i] = (byte) (i & 0xf);
        }
    }

    @Override
    public void test() {
        testNeon(A, B, COUNT);
    }

    /**
     * 测试c和neon，对比性能
     */
    public native void testNeon(int[] A, int[] B, int len);
}
