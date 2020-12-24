#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include "Print.h"

enum Transaction 
{
  READ_TRANSACTION,
  WRITE_TRANSACTION
};

class Persistence 
{
  public:
  virtual bool beginTransaction(uint8_t const transaction) = 0;
  virtual void commitTransaction(void);
  virtual void writeRecord(uint32_t const record, uint8_t const * datetime, uint8_t const datetime_size);

  virtual uint32_t getSize(void) = 0;
  virtual Print & getReader(void) = 0;
};

#endif /*PERSISTENCE_H_*/
