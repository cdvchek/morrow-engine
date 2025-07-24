#pragma once

#include "defines.h"

b8 input_init();

void input_update();

void input_process_button(input_code code, b8 pressed);
void input_process_mouse_move(i32 x, i32 y);
void input_process_mouse_scroll(i32 scroll);
void input_process_axis(axis_code code, f32 value);

b8 input_is_pressed(input_code code);
b8 input_is_held(input_code code);
b8 input_was_pressed(input_code code);
b8 input_was_released(input_code code);

i32 input_get_mouse_x();
i32 input_get_mouse_y();
i32 input_get_mouse_delta_x();
i32 input_get_mouse_delta_y();
i32 input_get_mouse_scroll();

f32 input_get_axis(axis_code code);