#include "renderer.h"

#include "core/logger.h"
#include "platform/platform.h"
#include "renderer/opengl/opengl_renderer.h"

static renderer* renderer_instance;
static renderer_backend_info info;

void renderer_init(renderer_backend_type backend) {
    platform_get_renderer_backend_info(&info);

    switch (backend) {
        case RENDERER_BACKEND_OPENGL:
            renderer_instance = opengl_create_renderer();
            break;

        default:
            LOG_FATAL("Unsupported renderer backend selected.");
            break;
    }
}
