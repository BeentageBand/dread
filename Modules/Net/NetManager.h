#ifndef NET_MANAGER_H
#define NET_MANAGER_H

#include <stdint.h>

#define MAX_ADDRESSES (16u)
class NetManager
{
  uint8_t table[MAX_ADDRESSES];
  uint8_t free_addresses;
  uint8_t idx;
  public:
  NetManager(void);
  int availableAddresses(void);
  bool contains(uint8_t address);
  uint8_t getFreeAddress(void);
  uint8_t nextAddress(void);
  uint8_t getAddress(void);
  uint8_t addAddress(uint8_t address);
};
#endif /*NET_MANAGER_H*/
