#include "input.h"

#include "core/memsys.h"
#include "platform/platform.h"

typedef enum input_code {
    M_LBUTTON,
    M_RBUTTON,
    M_MBUTTON,
    M_4BUTTON,
    M_5BUTTON,
    K_BACK,
    K_TAB,
    K_ENTER,
    K_PAUSE,
    K_CAPITAL,
    K_ESCAPE,
    K_SPACE,
    K_PAGE_UP,
    K_PAGE_DOWN,
    K_END,
    K_HOME,
    K_LEFT,
    K_UP,
    K_RIGHT,
    K_DOWN,
    K_INSERT,
    K_DELETE,
    K_PRINT,
    K_0,
    K_1,
    K_2,
    K_3,
    K_4,
    K_5,
    K_6,
    K_7,
    K_8,
    K_9,
    K_A,
    K_B,
    K_C,
    K_D,
    K_E,
    K_F,
    K_G,
    K_H,
    K_I,
    K_J,
    K_K,
    K_L,
    K_M,
    K_N,
    K_O,
    K_P,
    K_Q,
    K_R,
    K_S,
    K_T,
    K_U,
    K_V,
    K_W,
    K_X,
    K_Y,
    K_Z,
    K_LWIN,
    K_RWIN,
    K_NUM0,
    K_NUM1,
    K_NUM2,
    K_NUM3,
    K_NUM4,
    K_NUM5,
    K_NUM6,
    K_NUM7,
    K_NUM8,
    K_NUM9,
    K_MULTIPLY,
    K_ADD,
    K_SUBTRACT,
    K_DECIMAL,
    K_DIVIDE,
    K_NUM_ENTER,
    K_F1,
    K_F2,
    K_F3,
    K_F4,
    K_F5,
    K_F6,
    K_F7,
    K_F8,
    K_F9,
    K_F10,
    K_F11,
    K_F12,
    K_F13,
    K_F14,
    K_F15,
    K_F16,
    K_F17,
    K_F18,
    K_F19,
    K_F20,
    K_F21,
    K_F22,
    K_F23,
    K_F24,
    K_NUMLOCK,
    K_SCROLLLOCK,
    K_LSHIFT,
    K_RSHIFT,
    K_LCONTROL,
    K_RCONTROL,
    K_LMENU,
    K_RMENU,
    K_VOLUME_MUTE,
    K_VOLUME_DOWN,
    K_VOLUME_UP,
    K_MEDIA_NEXT,
    K_MEDIA_PREV,
    K_MEDIA_STOP,
    K_MEDIA_PLAY_PAUSE,
    K_GRAVE_TILDE,
    K_DASH_UNDERSCORE,
    K_EQUALS_PLUS,
    K_LBRACKET,
    K_RBRACKET,
    K_BACKSLASH_PIPE,
    K_SEMICOLON_COLON,
    K_APOSTROPHE_DBLQUOTE,
    K_COMMA_LESSTHAN,
    K_PERIOD_GREATERTHAN,
    K_FORWARDSLASH_QUESTION,
    C_A,
    C_B,
    C_X,
    C_Y,
    C_DPAD_LEFT,
    C_DPAD_UP,
    C_DPAD_RIGHT,
    C_DPAD_DOWN,
    C_LTHUMB_PRESS,
    C_RTHUMB_PRESS,
    C_LBUMPER,
    C_RBUMPER,
    C_LTRIGGER,
    C_RTRIGGER,
    C_PADDLE1,
    C_PADDLE2,
    C_PADDLE3,
    C_PADDLE4,
    C_VIEW,
    C_MENU,
    C_HOME,
    INPUT_CODE_COUNT
} input_code;

typedef enum axis_code {
    AXIS_LSTICK_X,
    AXIS_LSTICK_Y,
    AXIS_RSTICK_X,
    AXIS_RSTICK_Y,
    AXIS_LTRIGGER,
    AXIS_RTRIGGER,
    AXIS_CODE_COUNT
} axis_code;

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

b8 input_init() {
    memsys_zero(&input_state_instance, sizeof(input_state));
    return TRUE;
}

void input_update() {
    for (u32 i = 0; i < INPUT_CODE_COUNT; i++) {
        input_state_instance.buttons[i][FRAME_PREVIOUS] = input_state_instance.buttons[i][FRAME_CURRENT];
    }

    input_state_instance.mouse.delta_x = 0;
    input_state_instance.mouse.delta_y = 0;
    input_state_instance.mouse.scroll = 0;
}

void input_process_button(input_code code, b8 pressed) {
    if (pressed && !input_state_instance.buttons[code][FRAME_CURRENT]) {
        input_state_instance.last_pressed_time[code] = platform_get_absolute_time();
    }
    input_state_instance.buttons[code][FRAME_CURRENT] = pressed;
}

void input_process_mouse_move(i32 x, i32 y) {
    input_state_instance.mouse.delta_x = x - input_state_instance.mouse.x;
    input_state_instance.mouse.delta_y = y - input_state_instance.mouse.y;
    input_state_instance.mouse.x = x;
    input_state_instance.mouse.y = y;
}

void input_process_mouse_scroll(i32 scroll) {
    input_state_instance.mouse.scroll = scroll;
}

void input_process_axis(axis_code code, f32 value) {
    input_state_instance.axis[code] = value;
}

b8 input_is_down(input_code code) {
    return input_state_instance.buttons[code][FRAME_CURRENT];
}

b8 input_is_held(input_code code) {
    return input_state_instance.buttons[code][FRAME_CURRENT] && input_state_instance.buttons[code][FRAME_PREVIOUS];
}

b8 input_was_pressed(input_code code) {
    return input_state_instance.buttons[code][FRAME_CURRENT] && !input_state_instance.buttons[code][FRAME_PREVIOUS];
}

b8 input_was_released(input_code code) {
    return !input_state_instance.buttons[code][FRAME_CURRENT] && input_state_instance.buttons[code][FRAME_PREVIOUS];
}

i32 input_get_mouse_x() {
    return input_state_instance.mouse.x;
}

i32 input_get_mouse_y() {
    return input_state_instance.mouse.y;
}

i32 input_get_mouse_delta_x() {
    return input_state_instance.mouse.delta_x;
}

i32 input_get_mouse_delta_y() {
    return input_state_instance.mouse.delta_y;
}

i32 input_get_mouse_scroll() {
    return input_state_instance.mouse.scroll;
}

f32 input_get_axis(axis_code code) {
    return input_state_instance.axis[code];
}