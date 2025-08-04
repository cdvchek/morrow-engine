#include "renderer.h"

#include "core/logger.h"
#include "renderer/opengl/opengl_renderer.h"

static renderer* renderer_instance;

void renderer_init(renderer_backend backend) {
    switch (backend) {
        case RENDERER_BACKEND_OPENGL:
            renderer_instance = opengl_create_renderer();
            break;

        default:
            LOG_FATAL("Unsupported renderer backend selected.");
            break;
    }
}
