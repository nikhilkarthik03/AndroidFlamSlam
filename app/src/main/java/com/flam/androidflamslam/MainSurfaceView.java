package com.flam.androidflamslam;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.widget.TextView;

public class MainSurfaceView extends GLSurfaceView {
    MainRenderer mRenderer;

    public MainSurfaceView(Context context, android.util.AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        mRenderer = new MainRenderer(context);
        setRenderer(mRenderer);
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    public void setFpsTextView(TextView fpsText) {
        if (mRenderer != null) {
            mRenderer.setFpsTextView(fpsText);
        }
    }


    public MainRenderer getRenderer() {
        return mRenderer;
    }

    public void onDestroy() {
        mRenderer.onDestroy();
    }
}