#ifndef NG_CLIENT_H
#define NG_CLIENT_H

#include "Modules/Net/NetManager.h"
#include "Modules/Net/Socket.h"
#include "Support/Persistence/Persistence.h"
#include "Utilities/Print.h"

class NGClient {

  char const * client_name;
  Socket * socket;
  NetManager * net_manager;
  Persistence * persistence; 
  Print * logger;

  public:
  NGClient(char const * client_name, Socket & socket, NetManager & net_manager, 
      Persistence & persistence,
      Print & logger);
  void pollRecords(void);
  void scanNet(union Request & req);

  private:
  void handleError(union Response & res);
  void handleGetRecord(union Request & req, union Response & res);
  void handlePutAddress(union Request & req, union Response & res);
  void sendAck(union Request & req);

};
#endif /*NG_CLIENT_H*/
