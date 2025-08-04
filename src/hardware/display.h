#pragma once

#include "defines.h"

typedef struct visual_display_unit {
    u32 screen_left;
    u32 screen_right;
    u32 screen_bottom;
    u32 screen_top;

    u32 work_left;
    u32 work_right;
    u32 work_bottom;
    u32 work_top;

    u16 height;
    u16 width;
    f32 aspect_ratio;

    u16 refresh_rate;
    u8 bit_depth;
} visual_display_unit;