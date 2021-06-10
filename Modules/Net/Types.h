#ifndef ROUTER_TYPES_H_H
#define ROUTER_TYPES_H_H

#include <stdint.h>

#define NO_ADDRESS (-1)
#define BROADCAST_ADDRESS (0)

#define REQUEST_HEADER '@'
#define RESPONSE_HEADER '$'
#define MSG_FOOTER '!'

enum StatusCodes {
  ROUTER_OK = 0,
  ROUTER_TIMEOUT,
  ROUTER_REQUEST_NOT_FOUND,
  ROUTER_REQUEST_MALFORMED,
  ROUTER_ALREADY_REGISTERED,
  ROUTER_NOT_REGISTERED,
};

enum RequestTypes { REQUEST_ACK = 0, REQUEST_PUT_ADDRESS, REQUEST_GET_RECORD };

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

#endif /*ROUTER_TYPES_H_H*/
