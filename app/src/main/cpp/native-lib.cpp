#include <jni.h>
#include <string>

#include "camera_renderer.hpp"
#include "sokol.hpp"

sg_image camera_image{};
std::unique_ptr<CameraRenderer> camera_renderer;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_flam_androidflamslam_NativeBridge_init(
    JNIEnv *env,
    jclass clazz,
    jint width,
    jint height) {

  // 1. Initialize Sokol FIRST
  if (!init_gpu()) {
    return JNI_FALSE;
  }

  // 2. Then create resources
  sg_image_desc cam_desc{};
  cam_desc.width = width;
  cam_desc.height = height;
  cam_desc.sample_count = 1;
  cam_desc.usage.stream_update = true;
  cam_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
  cam_desc.label = "camera-image";

  camera_image = sg_make_image(&cam_desc);

  camera_renderer = std::make_unique<CameraRenderer>(width, height);

  return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_flam_androidflamslam_NativeBridge_onFrame(
    JNIEnv *env,
    jclass clazz /* this */) {
  camera_renderer->render(camera_image);
}