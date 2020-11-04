#ifndef FILE_PERSISTENCE_H
#define FILE_PERSISTENCE_H

#include "Persistence.h"
#include "Modules/SD/SD.h"
#include "Utilities/Print.h"

#define PERSISTENCE_FILENAME_SIZE 13U

class FilePersistence : public Persistence
{
  File * file;
  char file_name[PERSISTENCE_FILENAME_SIZE];
  Print * logger;

  public:
  FilePersistence(File & file, uint8_t const * file_prefix, Print & logger);
  bool beginTransaction(uint8_t const transaction);
  void commitTransaction(void);
  void writeRecord(uint32_t const record, uint8_t const * datetime, uint8_t const datetime_size);
  bool deprecateFilename(void);

  char const * getFilename(void);
  uint32_t getSize(void);
  Print & getReader(void) { return *file;}

  private:
  void generateNextUID(void);
};


#endif /*FILE_PERSISTENCE_H*/
