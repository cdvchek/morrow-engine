#pragma once

#include "defines.h"
#include "hardware/display.h"
#include "renderer/renderer.h"

b8 platform_init(u32 x, u32 y, u32 width, u32 height);
void platform_shutdown();

b8 platform_pump_messages();

f64 platform_get_absolute_time();

visual_display_unit* platform_get_display_list();

b8 platform_get_renderer_backend_info(renderer_backend_info* info);

void platform_sleep(u64 ms);

void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);

void* platform_allocate(u64 size, b8 aligned, u16 alignement);
void platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dest, const void* source, u64 size);
void* platform_set_memory(void* dest, i32 value, u64 size);
void* platform_move_memory(void* dest, const void* source, u64 size);
