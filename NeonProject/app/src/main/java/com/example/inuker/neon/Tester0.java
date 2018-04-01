package com.example.inuker.neon;

public class Tester0 extends AbsTester {

    @Override
    public void test() {
        testInstruction();
    }

    /**
     * 测试c和neon，对比性能
     */
    public native void testInstruction();
}
