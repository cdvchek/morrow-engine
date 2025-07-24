#include "app/application.h"

int main(void) {
    log_test();

    if (!app_init()) {
        return 2;
    }

    return !app_run();
}