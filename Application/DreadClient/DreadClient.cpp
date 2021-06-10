#include "DreadClient.h"
#include "HID.h"
#include "Locale.h"
#include <stdbool.h>

bool DreadClient::send_packet = false;

DreadClient::DreadClient(FilePersistence &persistence, Modem &modem,
                         Print &logger)
    : persistence(&persistence), modem(&modem), logger(&logger) {}

void DreadClient::CheckTime(void) { DreadClient::send_packet = true; }

void DreadClient::uploadFromPersistence(void) {
  HID &hid = HID::get();

  if (!modem->isConnected()) {
    hid.setLed(0);
  } else if (DreadClient::send_packet && uploadPacket() &&
             persistence->deprecateFilename()) {
    char const *filename = persistence->getFilename();
    modem->updatePacketName(filename, PERSISTENCE_FILENAME_SIZE);
    hid.setLed(0);
  } else {
    hid.setLed(0);
  }

  DreadClient::send_packet = false;
}

bool DreadClient::uploadPacket(void) {

  uint8_t list_length = 0; /*Variable that stores the slave book size. This is
                              list is uploaded when upload is successful*/
  bool result = persistence->beginTransaction(
      READ_TRANSACTION); /*Stores the return value of gprs methods.*/

  Locale &locale = Locale::get();
  static uint8_t datetime[DATETIME_SIZE];

  locale.getDateTime(datetime);
  uint32_t file_length =
      persistence
          ->getSize(); /*Variable that stores the backup file size. Up to 2^31*/

  // logger->printNumber(file_length,DEC);                  /*Prints
  // file_length, it will decrease with each packet.*/ logger->println();

  list_length += DATETIME_SIZE; /*add length of date + \r: 19 bytes*/

  result =
      modem->openPacketSession(); /*Preset result as true. It will turn false if
           modem instructions fails and it will cancel the uploading.*/
  while ((uint32_t)(file_length + (uint32_t)list_length) > 0UL && result) {
    uint16_t packet_size = (file_length >= 500U) ? 500U : file_length;

    result = modem->sendPacketSize(packet_size);
    modem->sendPacketData(persistence->getReader(), packet_size);
    file_length -= packet_size; /*Substract 500 from size*/
  }
  persistence->commitTransaction();

  logger->write('D');
  logger->println();

  result = result && modem->closePacketSession();
  // Uploading was successful when the file was totally sended
  return file_length == 0 && result;
}
