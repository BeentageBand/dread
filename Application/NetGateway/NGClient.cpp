#include "NGClient.h"
#include "Support/Util/Locale.h"
#include "Utilities/DreadSystem.h"

NGClient::NGClient(char const * client_name, Socket & socket, NetManager & net_manager, 
      Persistence & persistence,
      Print & logger)
  : client_name(client_name), socket(&socket), net_manager(&net_manager), 
  persistence(&persistence),
  logger(&logger)
{}

void NGClient::scanNet(union Request & req) 
{
  if (0 == net_manager->availableAddresses()) return;
  req.addr = BROADCAST_ADDRESS;
  req.type = REQUEST_PUT_ADDRESS;
  req.msg[0] = net_manager->getFreeAddress();
  socket->send(req);
}

void NGClient::pollRecords(void)
{
  static union Request req;
  scanNet(req);

  net_manager->nextAddress();
  if (net_manager->getAddress()) {
     req.addr = net_manager->getAddress();
     req.type = REQUEST_GET_RECORD;
     req.msg[0] = 0;
     socket->send(req);
  }

  static union Response res;
  uint8_t rc = socket->process(res, 1000);

  if (rc != SOCKET_MSG_RECEIVED) return;

  if (res.status != ROUTER_OK)
  {
    handleError(res);
    return;
  }

  switch(res.type)
  {
    case RESPONSE_GET_RECORD:
      handleGetRecord(req, res);
      break;
    case RESPONSE_PUT_ADDRESS:
      handlePutAddress(req, res);
      break;
    default:
      logger->write("Unhandled response");
  }
}

void NGClient::handleGetRecord(union Request & req, union Response & res)
{
  if(!net_manager->contains(res.addr)) return;

  delayMicroseconds(100);
  uint8_t result = persistence->beginTransaction(WRITE_TRANSACTION);
  if (result)
  {
    //Prints in file the following format: ID \t RFIDContent(as a decimal number) \t date (dd/mm/yyyy) \t hour (hh:mm) \t Serialnum uint32_t record = *reinterpret_cast<uint32_t *>(res.msg);
    static uint8_t datetime[DATETIME_SIZE + 1] = {0};
    uint32_t record = *reinterpret_cast<uint32_t*>(req.msg);

    Locale::get().getDateTime(datetime);
    persistence->writeRecord(record, datetime, DATETIME_SIZE);
    persistence->commitTransaction();
    delayMicroseconds(100);
    sendAck(req);
  }
}

void NGClient::handlePutAddress(union Request & req, union Response & res)
{
  if (net_manager->addAddress(res.addr))
  {
    sendAck(req);
  }
}

void NGClient::handleError(union Response & res)
{
  //TODO: handle errors
}

void NGClient::sendAck(union Request & req)
{
  req.type = REQUEST_ACK;
  socket->send(req);
}
