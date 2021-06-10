#include "NGServer.h"
#include "HID.h"

NGServer::NGServer(char const *server_name, Router &router,
                   Persistence &persistence, Print &logger)
    : address(NO_ADDRESS), server_name(server_name), router(&router),
      persistence(&persistence), logger(&logger) {}

void NGServer::listen(void) {
  static union Request request;
  static union Response response;

  logger->write(server_name);
  logger->write('\n');

  uint8_t rc = router->registerService(address, 1000);
  if (rc == ROUTER_NOT_REGISTERED || rc == ROUTER_TIMEOUT)
    return;

  rc = router->listenRequest(request, address, 1000);
  // logger->println(rc,DEC);
  if (rc == ROUTER_REQUEST_NOT_FOUND || rc == ROUTER_TIMEOUT) {
    HID::get().setLed(0); // TODO: net blink state
    return;
  }

  logger->write(request.raw, sizeof(request.raw));
  logger->write('\n');

  bool session = true;

  do {
    switch (request.type) {
    case REQUEST_ACK: {
      // persistence->flush(); TODO: wipe struct
      session = false;
      break;
    }
    case REQUEST_GET_RECORD: {

      response.status = ROUTER_OK;
      response.addr = address;
      response.type = RESPONSE_GET_RECORD;
      router->send(response);

      uint8_t *wptr = response.msg;
      Print &reader = persistence->getReader();

      while (wptr != (response.raw + 1) && reader.available()) {
        *wptr = reader.read();
        if (*wptr == '\n')
          break;
        wptr++;
      }

      if (0 == reader.available())
        session = false;

      router->send(response);
      break;
    }
    default: {
      logger->write("unhandled response");
      break;
    }
    }

  } while (session);
}
