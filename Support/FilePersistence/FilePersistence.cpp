#include "FilePersistence.h"
#include "HID.h"
#include "SPI.h"

#define SD_SS 10

FilePersistence::FilePersistence(File &file, uint8_t const *file_prefix,
                                 Print &logger)
    : file(&file), file_name(), logger(&logger) {
  HID::get().setLed(ORG_GRN);
  memcpy(file_name, file_prefix, PERSISTENCE_FILENAME_SIZE);
}

void FilePersistence::writeRecord(uint32_t const record,
                                  uint8_t const *datetime,
                                  uint8_t const datetime_size) {
  file->write((uint8_t *)file_name, 3);
  file->write('\t');

  file->write(record);
  file->write('\t');
  // Writes date to file->
  //
  file->write(datetime, datetime_size);

  // Print function way be able to print date correctly.
  // file->print(date);
  file->write('\t');

  file->write((uint8_t *)file_name, 3); // prefix

  file->println();
  file->close();
  // date[9]=',';//change SIM900 CLK format = "date   hour" to "date,hour".
  // Necessary for CCLK matching. Checks if file was correctly created. Is for
  // debugging purposes only.
  SD.exists(file_name);
  logger->write('S');
  logger->write('v');
  logger->println();
}

bool FilePersistence::beginTransaction(uint8_t const transaction) {
  SD.begin(SD_SS);

  uint8_t file_mode;

  switch (transaction) {
  case READ_TRANSACTION:
    file_mode = FILE_READ;
    break;
  case WRITE_TRANSACTION:
    file_mode = FILE_WRITE;
    break;
  default:
    return false;
  }

  *file = SD.open(file_name, file_mode);
  return *file;
}

void FilePersistence::commitTransaction(void) { file->close(); }

char const *FilePersistence::getFilename(void) { return file_name; }

uint32_t FilePersistence::getSize(void) { return file->size(); }

bool FilePersistence::deprecateFilename(void) {
  logger->write('R'); /*Write on debug "R*\r"*/
  logger->write('*');
  logger->println();

  SD.remove(file_name);

  HID &hid = HID::get();

  if (!SD.exists(
          file_name)) /*Removes the file with file_name. Check if exist to
              validate removal. Non existent = removed, existent =not removed */
  {
    // logger->println("genF*");
    generateNextUID();

    logger->write('D');
    logger->println();
    hid.setLed(LO_ALL);
    return true;
  } else {
    logger->write('U');
    logger->write('*');
    logger->println();
    hid.setLed(BLK_RED);
    return false;
  }

  SPI.end();
  SPI.begin();
}

void FilePersistence::generateNextUID(void) {
  uint8_t counter = 7U; /*Pointer will go backwards.File_name only changes first
                           8 uint8_tacters [0-7]*/
  do {
    switch (file_name[counter]) {
    case 'Z': /*'Z' to '0'. Change previous uint8_tacter."AZ" to "B0"*/
      file_name[counter] = '0';
      --counter;
      break;
    case '9': /*'9' to 'A'."A9" to "AA". Finish*/
      file_name[counter] = 'A';
      counter = 2U;
      break;
    default:
      ++file_name[counter]; /*Adds 1 to uint8_t, ASCII have alphanumeric order.
                               Finish*/
      counter = 2U;
      break;
    }

  } while (file_name[counter + 1] == '0' &&
           counter > 2U); /*Checks uint8_t overflow to continue with file
                             generation, up to 10*/
}
