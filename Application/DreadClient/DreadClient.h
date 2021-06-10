#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include "FilePersistence.h"
#include "Modem.h"
#include "Print.h"

class DreadClient {

  static bool send_packet;
  FilePersistence *persistence;
  Modem *modem;
  Print *logger;

public:
  DreadClient(FilePersistence &persistence, Modem &modem, Print &logger);

  static void CheckTime(void);
  void uploadFromPersistence(void);

private:
  bool uploadPacket(void);
};
#endif // !WEBCLIENT_H
