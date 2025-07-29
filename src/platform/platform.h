#pragma once

#include "../core/input.h"
#include "defines.h"

b8 platform_init();
void platform_shutdown();

b8 platform_pump_messages();

f64 platform_get_absolute_time();

void platform_sleep(u64 ms);

void platform_console_write(const char* message, u8 color);
void platform_console_write_error(const char* message, u8 color);

void* platform_allocate(u64 size, u16 alignement);
void platform_free(void* block, u16 alignment);
void* platform_zero_memory(void* block, u64 size);
void* platform_copy_memory(void* dest, const void* source, u64 size);
void* platform_set_memory(void* dest, i32 value, u64 size);
void* platform_move_memory(void* dest, const void* source, u64 size);
