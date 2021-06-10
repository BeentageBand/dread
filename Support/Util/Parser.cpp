#include "Parser.h"

uint16_t Parser::parseNumber(uint8_t const *buff, uint8_t const ndigit) {
  uint16_t n = 0;
  for (uint8_t i = 0; i < ndigit; ++i) {
    if (('0' <= buff[i]) || ('9' >= buff[i])) {
      n += (n * 10) + (buff[i] - '0');
    }
  }
  return n;
}

void Parser::printNumber(uint8_t *const dest, uint8_t size, uint16_t n) {
  while ((0 != n) && size--) {
    uint16_t div = n / 10;
    dest[size - 1] = (n - 10 * div) + '0';
    n = div;
  }
}
