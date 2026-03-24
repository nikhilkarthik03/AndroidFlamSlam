package com.flam.androidflamslam;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.SystemClock;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainRenderer implements GLSurfaceView.Renderer {

    Context context;
    private boolean _init = false;

    private int frameCount = 0;
    private long lastTime = 0;

    private TextView fpsText;

    public MainRenderer(Context ctx) {
        this.context = ctx;
    }

    public void setFpsTextView(TextView fpsText) {
        this.fpsText = fpsText;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        lastTime = SystemClock.elapsedRealtime();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        if (_init) return;
        NativeBridge.init(width, height);
        _init = true;
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        NativeBridge.onFrame();

        frameCount++;
        long now = SystemClock.elapsedRealtime();
        long diff = now - lastTime;

        if (diff >= 1000) {
            final int fps = (int)((frameCount * 1000f) / diff);

            if (fpsText != null) {
                fpsText.post(() -> fpsText.setText("FPS: " + fps));
            }

            frameCount = 0;
            lastTime = now;
        }
    }

    public void onDestroy() {}
}