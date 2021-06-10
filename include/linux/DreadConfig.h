#ifndef DREAD_CONFIG_H
#define DREAD_CONFIG_H

#include <stdint.h>
#define PROGMEM

#define pgm_read_byte(pgm_ptr) *((const uint8_t *)pgm_ptr)

#endif /*DREAD_CONFIG_H*/
