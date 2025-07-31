#pragma once

#include "defines.h"

typedef struct event_context {
    union {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;
} event_context;

typedef enum event_code {
    EVENT_CODE_KEY_PRESSED,
    EVENT_CODE_KEY_RELEASED,
    EVENT_CODE_MOUSE_MOVED,
    EVENT_CODE_MOUSE_WHEEL,
    EVENT_CODE_AXIS_CHANGE,
    EVENT_CODE_QUIT,
    EVENT_CODE_WINDOW_RESIZED,
    EVENT_CODE_COUNT
} event_code;

typedef b8 (*PFN_on_event)(event_code code, void* sender, void* listener, event_context context);

b8 event_init();
void event_shutdown();

b8 event_register(event_code code, void* listener, PFN_on_event on_event);
b8 event_unregister(event_code code, void* listener, PFN_on_event on_event);

b8 event_fire(event_code code, void* sender, event_context context);