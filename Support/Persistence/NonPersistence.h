#ifndef NON_PERSISTENCE_H
#define NON_PERSISTENCE_H
#include "Persistence.h"
#include "Stream.h"

class NonPersistence : public Persistence {
  Print *logger;
  Stream stream;

public:
  NonPersistence(Print &logger, char *const buffer, uint16_t const size);
  bool beginTransaction(uint8_t const transaction);
  void commitTransaction(void);
  void writeRecord(uint32_t const record, uint8_t const *datetime,
                   uint8_t const datetime_size);

  uint32_t getSize(void);
  Print &getReader(void);
};

#endif /*NON_PERSISTENCE_H*/
