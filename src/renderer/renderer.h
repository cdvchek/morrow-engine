#pragma once

#include "defines.h"

typedef struct renderer {
    void (*init)(void);
    void (*shutdown)(void);
    void (*set_clear_color)(float r, float g, float b, float a);
    void (*begin_frame)(void);
    void (*end_frame)(void);
    void (*draw_mesh)(void* mesh);
    void (*set_shader)(void* shader);
} renderer;

typedef enum renderer_backend {
    RENDERER_BACKEND_OPENGL,
    RENDERER_BACKEND_VULKAN,
    RENDERER_DIRECTX
} renderer_backend;

void renderer_init(renderer_backend backend);