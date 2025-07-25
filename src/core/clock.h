#pragma once

#include "defines.h"

typedef struct clock {
    f64 startTime;
    f64 elapsed;
} clock;

void clock_update(clock* clock);
void clock_start(clock* clock);
void clock_stop(clock* clock);