#include "CardReader.h"
#include "HID.h"
#include "Locale.h"

CardReader::CardReader(MifareLite &mifare, Persistence &persistence,
                       Print &logger)
    : mifare(&mifare), persistence(&persistence), logger(&logger), mfr_serial(),
      mfr_last_serial() {}

void CardReader::ReadAndStore(uint8_t *key, uint8_t const size) {
  static uint8_t buffer[16] = {0};
  bool result;
  HID &hid = HID::get();

  // Check the presence of a S150 Mifare Card
  if (mifare->Request(PICC_REQIDL, buffer) != MI_ERR) {
    // if a card was present, continues the S150 protocol.
    // Reads one (of all the) card(s) that are near the antenna.
    // The S150 read returns block 0, Serial IUD number.
    // In order to avoid saving the same card again, check if last serial number
    // is different from the new serial num. Last serial number is the last card
    // routine that was successfully saved on SD. logger->println("Req");
    result = mifare->Anticoll(mfr_serial) != MI_ERR;
    // PrintLong(mfr_serial,0);
    // PrintLong(mfr_last_serial,0);
    //#warning equalStr() was commented
    if (result && 0 == memcmp(mfr_serial, mfr_last_serial, SERIAL_SIZE)) {
      // logger->println("Anti");
      // Uses the serial number to select the S150 to communicate with.
      result = mifare->SelectTag(buffer, mfr_serial) != MI_ERR;
      if (result) {
        // logger->println("Tag");
        hid.beep();
        // Proceeds with authentication, Nedera uses KeyB, so the method is
        // PICC_AUTHENT1B.}
        result = mifare->Auth(PICC_AUTHENT1B, 0x04, key, buffer, mfr_serial) !=
                 MI_ERR;
        if (result) {
          // logger->println("Auth");
          // If success, reads block 0 sector 1. Mifare S150 memory is a vector
          // (not a matrix as how the doc talk about) So each sector has 4
          // blocks, so each 4 blocks the sector changes. the first 3 block of
          // each sector can be used as memory, the 4th one has the
          // Authentication configuration (keys and modes each block may
          // implement) of the whole sector. Avoid to read or write (EVEN
          // WORSE!!) the 4th block of each sector.
          result = mifare->Read(0x04, buffer) != MI_ERR;
          if (result) {
            // logger->println("Read");
            // After successfully reads the block, the S150 Card is sent back to
            // sleep.
            mifare->Halt();

            // Writing File Method
            result = persistence->beginTransaction(WRITE_TRANSACTION);
            if (result) {
              // Prints in file the following format: ID \t RFIDContent(as a
              // decimal number) \t date (dd/mm/yyyy) \t hour (hh:mm) \t
              // Serialnum
              uint32_t record = *reinterpret_cast<uint32_t *>(buffer);
              Locale::get().getDateTime(buffer);
              persistence->writeRecord(record, buffer, DATETIME_SIZE);
              persistence->commitTransaction();
            }
          }
        }
      }
    }

    if (result) {
      hid.successfulChime();
      memcpy(mfr_last_serial, mfr_serial, 5);
    } else {
      hid.failureChime();
      logger->write('U'); /*Print "Un"*/
      logger->write('n');
      logger->println();
    }
  }
  delay(500);
  // SPI.begin();
  // mifare->Init();
}
