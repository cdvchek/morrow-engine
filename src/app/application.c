#include "application.h"

#include "containers/darray.h"
#include "core/clock.h"
#include "core/event.h"
#include "core/input.h"
#include "core/logger.h"
#include "hardware/display.h"
#include "platform/platform.h"

typedef struct application_state {
    b8 is_running;
    clock clock;
    f64 last_time;
    visual_display_unit* displays;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

b8 application_on_key_press(event_code code, void* sender, void* listener, event_context context);
b8 application_on_mouse_move(event_code code, void* sender, void* listener, event_context context);
b8 application_on_quit(event_code code, void* sender, void* listener, event_context context);

b8 app_init() {
    if (initialized) {
        LOG_ERROR("application_init called more than once.");
        return FALSE;
    }
    if (!logger_init()) {
        LOG_ERROR("Logging initialization failed. External logs will not be recorded.");
    }

    app_state.displays = platform_get_display_list();

    // TODO: temporary assignment to my right screen at not fullscreen.
    u32 x = app_state.displays[1].work_left;
    u32 y = app_state.displays[1].work_top;
    u32 width = app_state.displays[1].work_right - app_state.displays[1].work_left;
    u32 height = app_state.displays[1].work_bottom - app_state.displays[1].work_top;

    if (!platform_init(x, y, width, height)) {
        LOG_FATAL("Platform initialization failed.");
        return FALSE;
    }

    if (!event_init()) {
        LOG_FATAL("Event initialization failed.");
        return FALSE;
    }

    if (!input_init()) {
        LOG_FATAL("Input initialization failed.");
        return FALSE;
    }

    event_register(EVENT_CODE_KEY_PRESSED, NULL, application_on_key_press);
    event_register(EVENT_CODE_MOUSE_MOVED, NULL, application_on_mouse_move);
    event_register(EVENT_CODE_QUIT, NULL, application_on_quit);

    app_state.is_running = TRUE;
    initialized = TRUE;

    return TRUE;
}

void app_shutdown() {
    event_unregister(EVENT_CODE_KEY_PRESSED, NULL, application_on_key_press);
    event_unregister(EVENT_CODE_MOUSE_MOVED, NULL, application_on_mouse_move);
    event_unregister(EVENT_CODE_QUIT, NULL, application_on_quit);

    event_shutdown();
    platform_shutdown();
    darray_destroy(app_state.displays);
    logger_shutdown();

    initialized = FALSE;
}

b8 app_run() {
    clock_start(&app_state.clock);
    clock_update(&app_state.clock);
    app_state.last_time = app_state.clock.elapsed;
    f64 target_frame_time = 1.0f / 144;
    u32 frame_count = 0;
    u8 frame_count_since_last_frame_log = 0;
    f64 debug_frames_log_delay = 1.0f;
    f64 time_since_last_frame_log = 0.0f;
    b8 log_fps = FALSE;

    while (app_state.is_running && platform_pump_messages()) {
        clock_update(&app_state.clock);
        f64 current_time = app_state.clock.elapsed;
        f64 delta = current_time - app_state.last_time;

        if (log_fps) {
            time_since_last_frame_log += delta;
            if (time_since_last_frame_log > debug_frames_log_delay) {
                LOG_INFO("FPS: %.2f", ((f64)(frame_count_since_last_frame_log) / time_since_last_frame_log));
                time_since_last_frame_log -= debug_frames_log_delay;
                frame_count_since_last_frame_log = 0;
            }
        }

        f64 frame_start = platform_get_absolute_time();
        // process_input()
        // update_game()
        // render_frame()
        f64 frame_end = platform_get_absolute_time();
        f64 frame_elapsed = frame_end - frame_start;
        f64 remaining_time = target_frame_time - frame_elapsed;

        if (remaining_time > 0) {
            u64 remaining_time_ms = (remaining_time * 1000);

            b8 limit_frames = FALSE;
            if (remaining_time_ms > 0 && limit_frames) {
                platform_sleep(remaining_time_ms - 1);
            }
        }

        frame_count++;
        if (log_fps) frame_count_since_last_frame_log++;
        app_state.last_time = current_time;
    }

    app_shutdown();

    return TRUE;
}

void log_test() {
    LOG_FATAL("A test message: %.2f", 3.14f);
    LOG_ERROR("A test message: %.2f", 3.14f);
    LOG_WARN("A test message: %.2f", 3.14f);
    LOG_INFO("A test message: %.2f", 3.14f);
    LOG_DEBUG("A test message: %.2f", 3.14f);
}

b8 application_on_mouse_move(event_code code, void* sender, void* listener, event_context context) {
    u32 x = context.data.i32[0];
    u32 y = context.data.i32[1];

    LOG_INFO("mouse move, x: %d, y: %d", x, y);
    return FALSE;
}

b8 application_on_key_press(event_code code, void* sender, void* listener, event_context context) {
    input_code key = context.data.u16[0];
    if (key == K_ESCAPE) {
        event_context context = {0};
        event_fire(EVENT_CODE_QUIT, NULL, context);
        return TRUE;
    }
    return FALSE;
}

b8 application_on_quit(event_code code, void* sender, void* listener, event_context context) {
    app_state.is_running = FALSE;
    return TRUE;
}