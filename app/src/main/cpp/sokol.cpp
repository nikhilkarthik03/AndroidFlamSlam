#define SOKOL_IMPL
#define SOKOL_GLES3
#define SOKOL_LOG_IMPL
#include "sokol.hpp"

bool init_gpu(){
    sg_environment env{};
    env.defaults.color_format = SG_PIXELFORMAT_RGBA8;
    env.defaults.depth_format = SG_PIXELFORMAT_DEPTH_STENCIL;
    env.defaults.sample_count = 1;
    env.metal.device = nullptr;
    env.d3d11.device = nullptr;
    env.wgpu.device = nullptr;

    sg_desc desc{};
    desc.environment = env;
    desc.logger.func = slog_func;
    sg_setup(&desc);
    return sg_isvalid();
}

void cleanup(){
    sg_shutdown();
}