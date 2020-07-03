#ifndef CARD_READER_H
#define CARD_READER_H

#include "Modules/RFID/MifareLite.h"
#include "Modules/SD/SD.h"
#include "Support/Persistence/Persistence.h"
#include "Utilities/Print.h"

#define SERIAL_SIZE (5U)

class CardReader
{
  MifareLite * mifare;
  Persistence * persistence;
  Print * logger;
  uint8_t mfr_serial[SERIAL_SIZE];
  uint8_t mfr_last_serial[SERIAL_SIZE];

  public:
  CardReader(MifareLite & mifare, Persistence & persistence, Print & logger);
  void ReadAndStore(uint8_t * key, uint8_t const size);
};

#endif /*CARD_READER_H*/
