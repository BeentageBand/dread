#include "DreadSystem.h"
#include <time.h>

unsigned long millis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_nsec / 1000000UL) + ts.tv_sec * 1000UL;
}