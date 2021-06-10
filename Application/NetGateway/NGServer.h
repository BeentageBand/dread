#ifndef NG_SERVER_H
#define NG_SERVER_H
#include "Persistence.h"
#include "Print.h"
#include "Router.h"

class NGServer {
  uint8_t address;
  char const *server_name;
  Router *router;
  Persistence *persistence;
  Print *logger;

public:
  NGServer(char const *server_name, Router &router, Persistence &persistence,
           Print &logger);
  void listen(void);
};
#endif /*NG_SERVER_H*/
