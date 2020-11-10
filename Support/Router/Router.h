#ifndef ROUTER_H
#define ROUTER_H
#include "Utilities/Print.h"
#include "Modules/RS485/RS485.h"

#define NO_ADDRESS (-1)
#define BROADCAST_ADDRESS (0)
#define MSG_BEGIN '@'
#define MSG_END '!'

enum RouterCodes {
  ROUTER_OK = 0,
  ROUTER_TIMEOUT,
  ROUTER_REQUEST_NOT_FOUND,
  ROUTER_REQUEST_MALFORMED,
  ROUTER_ALREADY_REGISTERED,
  ROUTER_NOT_REGISTERED,
};

enum RequestTypes {
  REQUEST_ACK = 0,
  REQUEST_PUT_ADDRESS,
  REQUEST_GET_RECORD
};

enum ResponseTypes {
  RESPONSE_ACK = 0,
  RESPONSE_PUT_ADDRESS,
  RESPONSE_GET_RECORD
};

union Request {
  struct {
    uint8_t addr;
    uint8_t type;
    uint8_t msg[1];
  };
  uint8_t raw[20];
};
union Response {
  struct {
    uint8_t addr;
    uint8_t type;
    uint8_t status;
    uint8_t msg[1];
  };
  uint8_t raw[20];
};

class Router 
{
  RS485 * port;
  Print * logger;
  public:
  Router(RS485 & port, Print & logger);
  uint8_t registerService(uint8_t & address, uint32_t const timeout);
  uint8_t listenRequest(union Request & req, uint8_t address, uint32_t const timeout);
  void send(union Response & res);
};
#endif /*ROUTER_H*/
