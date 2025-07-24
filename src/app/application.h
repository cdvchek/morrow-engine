#pragma once

#include "defines.h"

// app_init breakdown:
// Logger initialized
// Platform initialized
b8 app_init();

void app_shutdown();

// app_run breakdown:
// Starts the clock
// Runs main game loop
// In game loop: pump msgs and update clock
b8 app_run();

// log_test breakdown:
// Test function to make sure basic logging works
void log_test();