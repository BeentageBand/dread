#include "Router.h"
#include "Utilities/DreadSystem.h"

Router::Router(Socket& socket, Print & logger)
  : socket(&socket), logger(&logger)
{}

uint8_t Router::registerService(uint8_t & address, uint32_t const timeout)
{
  if (NO_ADDRESS != address) {
    return ROUTER_ALREADY_REGISTERED;
  }

  union Request req;
  uint8_t rc = listenRequest(req, address, timeout);

  if (SOCKET_MSG_RECEIVED == rc && BROADCAST_ADDRESS == req.addr &&
      REQUEST_PUT_ADDRESS == req.type) {
     address = req.msg[0];
     union Response res;
     res.status = ROUTER_OK;
     res.addr = address;
     send(res);
     return ROUTER_OK;
  } else {
     return ROUTER_NOT_REGISTERED;
  }
}

uint8_t Router::listenRequest(union Request & req, uint8_t address, uint32_t const timeout)
{
  uint8_t rc = socket->process(req, timeout);

  if (SOCKET_MSG_RECEIVED == rc || BROADCAST_ADDRESS == req.addr || address == req.addr) return SOCKET_MSG_RECEIVED;

  return rc;
}
