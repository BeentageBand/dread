#include "Router.h"

Router::Router(RS485 & port, Print & logger)
  : port(&port), logger(&logger)
{}

uint8_t Router::registerService(uint8_t & address, uint32_t const timeout)
{
  if (NO_ADDRESS != address) {
    return ROUTER_ALREADY_REGISTERED;
  }

  union Request req;
  uint8_t rc = listenRequest(req, address, timeout);

  if (ROUTER_OK == rc && BROADCAST_ADDRESS == req.addr &&
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
  //TODO: get correct code
  uint8_t rc = port->processResponse((char *)req.raw, MSG_BEGIN, MSG_END, sizeof(req.raw), timeout);

  if (ROUTER_OK == rc || BROADCAST_ADDRESS == req.addr || address == req.addr) return ROUTER_OK;

  return rc;
}

void Router::send(union Response & res)
{
  port->send((char *) res.raw, sizeof(res.raw));
}
  
