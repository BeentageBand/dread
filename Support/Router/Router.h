#ifndef ROUTER_H
#define ROUTER_H
#include "Print.h"
#include "RS485.h"
#include "Socket.h"

class Router {
  Socket *socket;
  Print *logger;

public:
  Router(Socket &socket, Print &logger);
  uint8_t registerService(uint8_t &address, uint32_t const timeout);
  uint8_t listenRequest(union Request &req, uint8_t address,
                        uint32_t const timeout);
  void send(union Response &res);
};
#endif /*ROUTER_H*/
