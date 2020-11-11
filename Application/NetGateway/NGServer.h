#ifndef NG_SERVER_H
#define NG_SERVER_H
#include "Support/Router/Router.h"
#include "Utilities/Print.h"
#include "Support/Persistence/Persistence.h"

class NGServer
{
  uint8_t address;
  char const * server_name;
  Router * router;
  Persistence * persistence;
  Print * logger;

  public:
  NGServer(char const * server_name, Router & router, Persistence & persistence, Print & logger);
  void listen(void);
};
#endif /*NG_SERVER_H*/