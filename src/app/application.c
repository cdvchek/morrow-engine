#include "application.h"

#include "core/clock.h"
#include "core/event.h"
#include "core/logger.h"
#include "platform/platform.h"

typedef struct application_state {
    b8 is_running;
    clock clock;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

b8 app_init() {
    if (initialized) {
        LOG_ERROR("application_init called more than once.");
        return FALSE;
    }
    if (!logger_init()) {
        LOG_ERROR("Logging initialization failed. External logs will not be recorded.");
    }

    if (!platform_init()) {
        LOG_FATAL("Platform initialization failed.");
        return FALSE;
    }

    if (!event_init()) {
        LOG_FATAL("Event initialization failed.");
        return FALSE;
    }

    app_state.is_running = TRUE;
    initialized = TRUE;

    return TRUE;
}

void app_shutdown() {
    event_shutdown();
    platform_shutdown();
    logger_shutdown();
    initialized = FALSE;
    app_state.is_running = FALSE;
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

    return TRUE;
}

void log_test() {
    LOG_FATAL("A test message: %.2f", 3.14f);
    LOG_ERROR("A test message: %.2f", 3.14f);
    LOG_WARN("A test message: %.2f", 3.14f);
    LOG_INFO("A test message: %.2f", 3.14f);
    LOG_DEBUG("A test message: %.2f", 3.14f);
}