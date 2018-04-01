package com.example.inuker.neon;

import junit.framework.Test;

public abstract class AbsTester implements Tester {

    public final String TAG = getClass().getSimpleName();

    static {
        System.loadLibrary("native-lib");
    }

    public void prepare() {

    }
}
