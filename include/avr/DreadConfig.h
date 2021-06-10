#ifndef DREAD_CONFIG_H
#define DREAD_CONFIG_H

#define F_CPU 8000000L /*Dread operates at 8MHz*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h> /*include io SFR definitions */
#include <avr/pgmspace.h>
#include <stdarg.h>
#include <stdio.h>

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define constrain(amt, low, high)                                              \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define round(x) ((x) >= 0 ? (long)((x) + 0.5) : (long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x) * (x))

#endif /*DREAD_CONFIG_H*/
