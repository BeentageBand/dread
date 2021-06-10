#include "NonPersistence.h"

NonPersistence::NonPersistence(Print &logger, char *const buffer,
                               uint16_t const size)
    : logger(&logger), stream(buffer, size) {}

bool NonPersistence::beginTransaction(uint8_t const transaction) {
  return 0 == stream.getWriteError();
}

void NonPersistence::commitTransaction(void) {}

void NonPersistence::writeRecord(uint32_t const record, uint8_t const *datetime,
                                 uint8_t const datetime_size) {
  if (0 == stream.getWriteError())
    return;
  stream.write(record);
  stream.write('\t');
  stream.write(datetime, datetime_size);
  stream.write('\n');
}

uint32_t NonPersistence::getSize(void) { return stream.available(); }

Print &NonPersistence::getReader(void) { return stream; }
