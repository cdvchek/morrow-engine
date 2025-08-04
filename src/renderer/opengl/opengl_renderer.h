#pragma once

#include "defines.h"
#include "renderer/renderer.h"

void opengl_init();
void opengl_shutdown();
void opengl_set_clear_color(float r, float g, float b, float a);
void opengl_begin_frame();
void opengl_end_frame();
void opengl_draw_mesh(void* mesh);
void opengl_set_shader(void* shader);

renderer* opengl_create_renderer();