#ifndef STREAM_H
#define STREAM_H
#include "Print.h"

class Stream : public Print {
  char *buffer;
  uint16_t mask;
  uint16_t hd;
  uint16_t tl;

public:
  Stream(char *buffer, uint16_t const mask)
      : buffer(buffer), mask(mask), hd(0), tl(0) {}

  int read(void) {
    if (hd == tl)
      return -1;
    int r = buffer[hd];
    hd = (hd + 1) & (mask - 1);
    return r;
  }

  void write(uint8_t const n) {
    uint16_t idx = (tl + 1) & (mask - 1);
    if (hd == idx)
      setWriteError(1);
    buffer[tl] = n;
    tl = idx;
  }
  void write(const uint8_t *buffer, uint16_t size) {
    while (size-- && 0 == getWriteError())
      write(*(buffer++));
  }

  int available(void) { return (hd > tl) ? mask - hd + tl : tl - hd; }

  void flush(void) { hd = tl = 0; }
};

#endif /*STREAM_H*/
