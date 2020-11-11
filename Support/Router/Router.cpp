#include "Router.h"
#include "Utilities/DreadSystem.h"

Router::Router(Print & port, Print & logger)
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
  uint8_t rc = processResponse(req.raw, MSG_BEGIN, MSG_END, sizeof(req.raw), timeout);

  if (ROUTER_OK == rc || BROADCAST_ADDRESS == req.addr || address == req.addr) return ROUTER_OK;

  return rc;
}

void Router::send(union Response & res)
{
  port->write(res.raw, sizeof(res.raw));
}
  
uint8_t Router::processResponse(uint8_t * response, uint8_t const header,
      uint8_t const footer,
      uint16_t const response_size,
      uint32_t const timeout)
{

      uint8_t pointer=0;
      int init_time;
      int time;
      memset(response,0,response_size); //Set to zeros the response (clean buffer)
      //Wait until bytes come from the
      init_time=millis();
      do
      {
              if(port->available()>0)
              {
                      response[pointer]=port->read();//Read until initial character match
                      logger->write(response[pointer]);
              }
              time=millis()-init_time;
      } while (response[pointer]!=header && time<timeout);
      logger->write("first matched");
      do
      {
              if(port->available()>0)
              {
                      response[pointer]=port->read();//Read until initial character match
                      //logger->write(response[pointer]);
                      pointer++;
              }
              time=millis()-init_time;
      } while (response[pointer-1]!=footer && pointer<response_size && time<timeout);
      logger->write("Time:\n");
      logger->write(time);
      //Return if the message matches length,initial and final characters, in other words, matches format.
      if (pointer==response_size && response[response_size-1]==footer)
      {
              return SC_RESPOND_RECEIVED; //Response matches format
      }
      else if(pointer>=response_size)
      {
              return SC_RESPOND_OUTOFBOUND;
      }
      else if (time>=timeout)
      {
              return SC_RESPOND_TIMEOUT;
      }
      else
      {
              return SC_RESPOND_NOFORMAT; //Receives a
      }
      logger->write("Left on port:");
      logger->write(port->available());
}
