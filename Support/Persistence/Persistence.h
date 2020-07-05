#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "Modules/SD/SD.h"
#include "Utilities/Print.h"

#define PERSISTENCE_FILENAME_SIZE 13U

enum Transaction {
  READ_TRANSACTION,
  WRITE_TRANSACTION
};

class Persistence
{
  File * file;
  char file_name[PERSISTENCE_FILENAME_SIZE];
  Print * logger;

  public:
  Persistence(File & file, uint8_t const * file_prefix, Print & logger);
  bool beginTransaction(uint8_t const transaction);
  void commitTransaction(void);
  void writeRecord(uint32_t const record, uint8_t const * datetime, uint8_t const datetime_size);
  bool deprecateFilename(void);

  char const * getFilename(void);
  uint32_t getSize(void);
  inline Print & getReader(void) { return *file;}

  private:
  void generateNextUID(void);
};


#endif /*PERSISTENCE_H*/
