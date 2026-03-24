package com.flam.androidflamslam;

public class NativeBridge {

    static {
        System.loadLibrary("androidflamslam");
    }

    public static native boolean init(int width, int height);

    public static native void onFrame();
}
