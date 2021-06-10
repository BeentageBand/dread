#include "DreadSystem.h"
#include <time.h>

unsigned long millis(void) {
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return (ts.tv_nsec / 1000000UL) + ts.tv_sec * 1000UL;
}

void digitalWrite(uint8_t pin, uint8_t val) {}
void pinMode(uint8_t pin, uint8_t mode) {}
void delay(unsigned long ms) {}
