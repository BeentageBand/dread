#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>

class Parser {
public:
  static uint16_t parseNumber(uint8_t const *buff, uint8_t const ndigit);
  static void printNumber(uint8_t *const dest, uint8_t const size,
                          uint16_t const n);
};

#endif /*PARSER_H*/
