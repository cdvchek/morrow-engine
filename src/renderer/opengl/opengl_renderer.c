#include "opengl_renderer.h"

#include "core/memsys.h"

void opengl_init() {
}
void opengl_shutdown() {
}
void opengl_set_clear_color(float r, float g, float b, float a) {
}
void opengl_begin_frame() {
}
void opengl_end_frame() {
}
void opengl_draw_mesh(void* mesh) {
}
void opengl_set_shader(void* shader) {
}

renderer* opengl_create_renderer() {
    renderer* opengl_renderer = (renderer*)memsys_allocate(sizeof(renderer), MEMSYS_CODE_RENDERER, FALSE, 0);
    opengl_renderer->init = opengl_init;
    opengl_renderer->shutdown = opengl_shutdown;
    opengl_renderer->set_clear_color = opengl_set_clear_color;
    opengl_renderer->begin_frame = opengl_begin_frame;
    opengl_renderer->end_frame = opengl_end_frame;
    opengl_renderer->draw_mesh = opengl_draw_mesh;
    opengl_renderer->set_shader = opengl_set_shader;
    return opengl_renderer;
}