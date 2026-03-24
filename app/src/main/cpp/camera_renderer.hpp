#ifndef ANDROIDFLAMSLAM_CAMERA_RENDERER_HPP
#define ANDROIDFLAMSLAM_CAMERA_RENDERER_HPP

#include <sokol_gfx.h>
#include "shaders/quad.glsl.h"

class CameraRenderer {
 private:
  int _width;
  int _height;
  bool _init;

  sg_pass _pass{};
  sg_pipeline _pip{};
  sg_view _view{};
  sg_bindings _bind{};
 public:
  CameraRenderer(int width, int height) : _width(width), _height(height), _init(true) {

    _pass.action.colors[0].load_action = SG_LOADACTION_CLEAR;
    _pass.action.colors[0].clear_value = {0.0f, 0.0f, 0.0f, 1.0f};
    _pass.swapchain.width = _width;
    _pass.swapchain.height = _height;
    _pass.swapchain.sample_count = 1;
    _pass.swapchain.color_format = SG_PIXELFORMAT_RGBA8;
    _pass.swapchain.depth_format = SG_PIXELFORMAT_DEPTH;
    _pass.swapchain.depth_format = SG_PIXELFORMAT_DEPTH_STENCIL;

    sg_pipeline_desc pip_desc{};
    pip_desc.shader = sg_make_shader(quad_shader_desc(sg_query_backend()));
    pip_desc.layout.buffers[0].stride = 20;
    pip_desc.layout.attrs[ATTR_quad_position].format = SG_VERTEXFORMAT_FLOAT3;
    pip_desc.layout.attrs[ATTR_quad_position].offset = 0;
    pip_desc.layout.attrs[ATTR_quad_texCords].format = SG_VERTEXFORMAT_FLOAT2;
    pip_desc.layout.attrs[ATTR_quad_texCords].offset = 12;
    pip_desc.index_type = SG_INDEXTYPE_UINT16;
    pip_desc.label = "camera-pipeline";
    _pip = sg_make_pipeline(&pip_desc);

    const float vertices[] = {
        // Position (x, y, z)       // Texture coordinates (u, v) - flipped Y
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left corner (was 0.0f, 0.0f)
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left corner (was 0.0f, 1.0f)
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom-right corner (was 1.0f, 1.0f)
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f    // top-right corner (was 1.0f, 0.0f)
    };

    const uint16_t indices[] = {
        0, 1, 2, // first triangle (top-left, bottom-left, bottom-right)
        0, 2, 3  // second triangle (top-left, bottom-right, top-right)
    };

    sg_buffer_desc vbuff_desc = {};
    vbuff_desc.data.ptr = vertices;
    vbuff_desc.data.size = sizeof(vertices);
    vbuff_desc.usage.vertex_buffer = true;
    vbuff_desc.usage.immutable = true;
    vbuff_desc.label = "main-vertices";

    sg_buffer_desc ibuff_desc = {};
    ibuff_desc.data.ptr = indices;
    ibuff_desc.data.size = sizeof(indices);
    ibuff_desc.usage.index_buffer = true;
    ibuff_desc.usage.immutable = true;
    ibuff_desc.label = "main-indices";

    sg_sampler_desc sampler_desc{};
    sampler_desc.min_filter = SG_FILTER_LINEAR;
    sampler_desc.mag_filter = SG_FILTER_LINEAR;
    sampler_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    sampler_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    sg_sampler smp = sg_make_sampler(&sampler_desc);

    sg_buffer vbuffer = sg_make_buffer(&vbuff_desc);
    sg_buffer ibuffer = sg_make_buffer(&ibuff_desc);

    _bind.vertex_buffers[0] = vbuffer;
    _bind.index_buffer = ibuffer;
    _bind.samplers[SMP_smp] = smp;
  }

  ~CameraRenderer() = default;

  void render(sg_image &img) {

    if (!_init) return;

    if (_view.id > 0) sg_destroy_view(_view);

    sg_view_desc view_desc{};
    view_desc.texture.image = img;
    view_desc.label = "camera-view";
    _view = sg_make_view(&view_desc);

    _bind.views[VIEW_tex] = _view;

    sg_begin_pass(_pass);
    sg_apply_pipeline(_pip);
    sg_apply_bindings(_bind);
    sg_draw(0, 6, 1);
    sg_end_pass();

    sg_commit();
  }
};
#endif //ANDROIDFLAMSLAM_CAMERA_RENDERER_HPP
