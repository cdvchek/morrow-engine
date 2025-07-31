#include "input.h"

#include "core/event.h"
#include "core/memsys.h"
#include "platform/platform.h"

typedef struct mouse_state {
    i32 x;
    i32 y;

    f32 delta_x;
    f32 delta_y;

    i32 scroll;
} mouse_state;

typedef enum {
    FRAME_PREVIOUS,
    FRAME_CURRENT,
    INPUT_FRAME_COUNT
} input_frame;

typedef struct input_state {
    b8 buttons[INPUT_CODE_COUNT][INPUT_FRAME_COUNT];
    f64 last_pressed_time[INPUT_CODE_COUNT];
    f32 axis[AXIS_CODE_COUNT];
    mouse_state mouse;
} input_state;

static input_state input_state_instance;
static b8 is_initialized = FALSE;

b8 input_init() {
    memsys_zero(&input_state_instance, sizeof(input_state));
    is_initialized = TRUE;
    return TRUE;
}

void input_update() {
    if (!is_initialized) return;
    for (u32 i = 0; i < INPUT_CODE_COUNT; i++) {
        input_state_instance.buttons[i][FRAME_PREVIOUS] = input_state_instance.buttons[i][FRAME_CURRENT];
    }

    input_state_instance.mouse.delta_x = 0;
    input_state_instance.mouse.delta_y = 0;
    input_state_instance.mouse.scroll = 0;
}

void input_process_button(input_code code, b8 pressed) {
    if (!is_initialized) return;
    if (pressed && !input_state_instance.buttons[code][FRAME_CURRENT]) {
        input_state_instance.last_pressed_time[code] = platform_get_absolute_time();
    }
    input_state_instance.buttons[code][FRAME_CURRENT] = pressed;

    event_code event = pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED;
    event_context context = {0};
    context.data.u16[0] = code;
    event_fire(event, NULL, context);
}

void input_process_mouse_move(i32 x, i32 y) {
    if (!is_initialized) return;
    input_state_instance.mouse.delta_x = x - input_state_instance.mouse.x;
    input_state_instance.mouse.delta_y = y - input_state_instance.mouse.y;
    input_state_instance.mouse.x = x;
    input_state_instance.mouse.y = y;

    event_context context = {0};
    context.data.i32[0] = x;
    context.data.i32[1] = y;
    event_fire(EVENT_CODE_MOUSE_MOVED, NULL, context);
}

void input_process_mouse_scroll(i32 scroll) {
    if (!is_initialized) return;
    input_state_instance.mouse.scroll = scroll;

    event_context context = {0};
    context.data.i32[0] = scroll;
    event_fire(EVENT_CODE_MOUSE_WHEEL, NULL, context);
}

void input_process_axis(axis_code code, f32 value) {
    if (!is_initialized) return;
    input_state_instance.axis[code] = value;

    event_context context = {0};
    context.data.u32[0] = code;
    context.data.f32[0] = value;
    event_fire(EVENT_CODE_AXIS_CHANGE, NULL, context);
}

b8 input_is_down(input_code code) {
    if (!is_initialized) return FALSE;
    return input_state_instance.buttons[code][FRAME_CURRENT];
}

b8 input_is_held(input_code code) {
    if (!is_initialized) return FALSE;
    return input_state_instance.buttons[code][FRAME_CURRENT] && input_state_instance.buttons[code][FRAME_PREVIOUS];
}

b8 input_was_pressed(input_code code) {
    if (!is_initialized) return FALSE;
    return input_state_instance.buttons[code][FRAME_CURRENT] && !input_state_instance.buttons[code][FRAME_PREVIOUS];
}

b8 input_was_released(input_code code) {
    if (!is_initialized) return FALSE;
    return !input_state_instance.buttons[code][FRAME_CURRENT] && input_state_instance.buttons[code][FRAME_PREVIOUS];
}

i32 input_get_mouse_x() {
    if (!is_initialized) return 0;
    return input_state_instance.mouse.x;
}

i32 input_get_mouse_y() {
    if (!is_initialized) return 0;
    return input_state_instance.mouse.y;
}

i32 input_get_mouse_delta_x() {
    if (!is_initialized) return 0;
    return input_state_instance.mouse.delta_x;
}

i32 input_get_mouse_delta_y() {
    if (!is_initialized) return 0;
    return input_state_instance.mouse.delta_y;
}

i32 input_get_mouse_scroll() {
    if (!is_initialized) return 0;
    return input_state_instance.mouse.scroll;
}

f32 input_get_axis(axis_code code) {
    if (!is_initialized) return 0;
    return input_state_instance.axis[code];
}