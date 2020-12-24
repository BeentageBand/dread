#ifndef NG_SERVER_H
#define NG_SERVER_H
#include "Router.h"
#include "Print.h"
#include "Persistence.h"

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
