#include "event.h"

#include "containers/darray.h"
#include "core/logger.h"
#include "core/memsys.h"

typedef enum event_code {
    EVENT_CODE_KEY_PRESSED,
    EVENT_CODE_KEY_RELEASED,
    EVENT_CODE_QUIT,
    EVENT_CODE_WINDOW_RESIZED
} event_code;

typedef struct registered_event {
    void* listener;
    PFN_on_event callback;
} registered_event;

typedef struct event_callback_list {
    registered_event* events;
} event_callback_list;

#define MAX_MESSAGE_CODES 16384

typedef struct event_system_state {
    event_callback_list registered_events[MAX_MESSAGE_CODES];
} event_system_state;

static b8 is_initialized = FALSE;
static event_system_state state;

b8 event_init() {
    if (is_initialized) {
        LOG_WARN("event_init() called after system already initialized.");
        return FALSE;
    }

    memsys_zero(&state, sizeof(state));
    is_initialized = TRUE;

    return TRUE;
}

void event_shutdown() {
    if (!is_initialized) {
        LOG_WARN("event_shutdown() called before event_init().");
        return;
    }

    for (u64 i = 0; i < MAX_MESSAGE_CODES; i++) {
        if (state.registered_events[i].events) {
            darray_destroy(state.registered_events[i].events);
            state.registered_events[i].events = NULL;
        }
    }

    is_initialized = FALSE;
}

b8 event_register(u16 code, void* listener, PFN_on_event on_event) {
    if (!is_initialized) {
        LOG_WARN("event_register() called before event_init().");
        return FALSE;
    }

    if (!state.registered_events[code].events) {
        state.registered_events[code].events = darray_create(registered_event);
    }

    // loop through and see if this listener and on_event callback are already registered
    u64 registered_count = darray_length(state.registered_events[code].events);
    for (u64 i = 0; i < registered_count; i++) {
        if (state.registered_events[code].events[i].callback == on_event && state.registered_events[code].events[i].listener == listener) {
            LOG_WARN("A event registration failed because of duplication.");
            return FALSE;
        }
    }

    // if no duplicate found, register event

    registered_event event;
    event.callback = on_event;
    event.listener = listener;
    darray_push(state.registered_events[code].events, event);

    return TRUE;
}

b8 event_unregister(u16 code, void* listener, PFN_on_event on_event) {
    if (!is_initialized) {
        LOG_WARN("event_unregister() called before event_init().");
        return FALSE;
    }

    registered_event* event;
    u64 registered_count = darray_length(state.registered_events[code].events);
    for (u64 i = 0; i < registered_count; i++) {
        if (state.registered_events[code].events[i].callback == on_event && state.registered_events[code].events[i].listener == listener) {
            darray_remove(state.registered_events[code].events, event, i);
        }
    }

    return TRUE;
}

b8 event_fire(u16 code, void* sender, event_context context) {
    if (!is_initialized) {
        LOG_WARN("event_fire() called before event_init()");
        return FALSE;
    }

    u64 registered_count = darray_length(state.registered_events[code].events);
    for (u64 i = 0; i < registered_count; i++) {
        registered_event e = state.registered_events[code].events[i];
        if (e.callback(code, sender, e.listener, context)) return TRUE;
    }

    return TRUE;
}