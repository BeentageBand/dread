#include "NGServer.h"
#include "Modules/HID/HID.h"

NGServer::NGServer(char const * server_name, Router & router, Persistence & persistence, Print & logger)
  : address(NO_ADDRESS), server_name(server_name), router(&router), persistence(&persistence), logger(&logger)
{}

void NGServer::listen(void)
{
  static union Request request;
  static union Response response;

  logger->write(server_name);
  logger->write('\n');

  uint8_t sc_result = router->registerService(address, 1000);
  if (sc_result == ROUTER_NOT_REGISTERED || 
      sc_result == ROUTER_TIMEOUT) return;

  sc_result = router->listenRequest(request, address, 1000);
  //logger->println(sc_result,DEC);
  if(sc_result == ROUTER_REQUEST_NOT_FOUND || 
     sc_result == ROUTER_TIMEOUT)
  {
    HID::get().setLed(0);//TODO: net blink state
    return;
  }

  logger->write(request.raw, sizeof(request.raw));
  logger->write('\n');

  bool session = true;

  do {
    switch(request.type)
    {
      case REQUEST_ACK: {
        //persistence->flush(); TODO: wipe struct
        session = false;
        break;
      }
      case REQUEST_GET_RECORD: {

        response.status = ROUTER_OK;
        response.addr = address;
        response.type = RESPONSE_GET_RECORD;
        router->send(response);

        uint8_t * wptr = response.msg;
        Print & reader = persistence->getReader();

        while(wptr != (response.raw + 1) && reader.available()) 
        {
          *wptr = reader.read();
          if (*wptr == '\n') break;
          wptr++;
        }

        if (0 == reader.available()) session = false;

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

