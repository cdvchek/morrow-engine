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

typedef b8 (*PFN_on_event)(u16 code, void* sender, void* listener, event_context context);

b8 event_init();
void event_shutdown();

b8 event_register(u16 code, void* listener, PFN_on_event on_event);
b8 event_unregister(u16 code, void* listener, PFN_on_event on_event);

b8 event_fire(u16 code, void* sender, event_context context);