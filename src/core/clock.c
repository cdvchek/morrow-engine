#include "clock.h"

#include "platform/platform.h"

void clock_update(clock* clock) {
    if (clock->startTime != 0) {
        clock->elapsed = platform_get_absolute_time() - clock->startTime;
    }
}

void clock_start(clock* clock) {
    clock->startTime = platform_get_absolute_time();
    clock->elapsed = 0;
}

void clock_stop(clock* clock) {
    clock->startTime = 0;
}