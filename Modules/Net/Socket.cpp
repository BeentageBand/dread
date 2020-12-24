#include <string.h>
#include "Socket.h"
#include "DreadSystem.h"

Socket::Socket(Print & port, Print & logger)
  : port(&port), logger(&logger)
{}

void Socket::send(union Request & req)
{
  port->write(req.raw, sizeof(req.raw));
}
void Socket::send(union Response & res)
{
  port->write(res.raw, sizeof(res.raw));
}

uint8_t Socket::process(union Request & req, uint32_t timeout)
{
  return listenMsg(req.raw, REQUEST_HEADER, MSG_FOOTER, sizeof(req.raw), timeout);
}

uint8_t Socket::process(union Response & res, uint32_t timeout)
{
  return listenMsg(res.raw, REQUEST_HEADER, MSG_FOOTER, sizeof(res.raw), timeout);
}

uint8_t Socket::listenMsg(uint8_t * msg, uint8_t header, uint8_t footer, uint16_t length, uint32_t timeout)
{
  uint8_t pointer=0;
  int init_time;
  int time;
  memset(msg, 0, length); //Set to zeros the msg (clean buffer)
  //Wait until bytes come from the
  init_time=millis();
  do
  {
          if(port->available()>0)
          {
                  msg[pointer]=port->read();//Read until initial character match
                  logger->write(msg[pointer]);
          }
          time=millis()-init_time;
  } while (msg[pointer]!=header && time<timeout);
  logger->write("first matched");
  do
  {
          if(port->available()>0)
          {
                  msg[pointer]=port->read();//Read until initial character match
                  //logger->write(msg[pointer]);
                  pointer++;
          }
          time=millis()-init_time;
  } while (msg[pointer-1]!=footer && pointer<length && time<timeout);
  logger->write("Time:\n");
  logger->write(time);
  //Return if the message matches length,initial and final characters, in other words, matches format.
  if (pointer==length && msg[length-1]==footer)
  {
          return SOCKET_MSG_RECEIVED; //Response matches format
  }
  else if(pointer>=length)
  {
          return SOCKET_MSG_OUTOFBOUND;
  }
  else if (time>=timeout)
  {
          return SOCKET_MSG_TIMEOUT;
  }
  else
  {
          return SOCKET_MSG_NOFORMAT; //Receives a
  }
  logger->write("Left on port:");
  logger->write(port->available());
}
