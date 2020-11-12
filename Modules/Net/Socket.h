#ifndef SOCKET_H
#define SOCKET_H

#include "Types.h"
#include "Utilities/Print.h"

enum SocketCodes {
  SOCKET_MSG_RECEIVED = 0,
  SOCKET_MSG_TIMEOUT,
  SOCKET_MSG_NOFORMAT,
  SOCKET_MSG_OUTOFBOUND
};

class Socket
{
  Print * port;
  Print * logger;
  public:
  Socket(Print & port, Print & logger);

  void send(union Request & req);
  void send(union Response & req);

  uint8_t process(union Request & req, uint32_t timeout);
  uint8_t process(union Response & req, uint32_t timeout);

  private:
  uint8_t listenMsg(uint8_t * msg, uint8_t header, uint8_t footer, uint16_t length, uint32_t timeout);
};

#endif /*SOCKET_H*/
