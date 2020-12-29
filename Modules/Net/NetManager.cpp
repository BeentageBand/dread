#include "NetManager.h"
#include <string.h>

NetManager::NetManager(void)
{
  memset(table, 0, sizeof(table));
  free_addresses = sizeof(table);
  idx = 0;
}

int NetManager::availableAddresses(void)
{
  return free_addresses;
}

uint8_t NetManager::getFreeAddress(void)
{
  for (uint8_t i = 0; i < MAX_ADDRESSES; ++i) 
  {
    if (table[i]) return table[i];
  }
  return -1;
}

uint8_t NetManager::nextAddress(void)
{

  uint8_t i = idx + 1;
  for (; i != idx; i = (i+1) % MAX_ADDRESSES)
  {
    if (table[i]) break;
  }

  if (i != idx) {
    return true;
  } else {
    return false;
  }
}

uint8_t NetManager::getAddress(void)
{
  return table[idx];
}

bool NetManager::contains(uint8_t address) {
  uint8_t h; 
  uint8_t probe = 0;
  do {
    h = (address + probe) % MAX_ADDRESSES ;
    if (table[h] == address) return true;
    if (table[h]) ++probe;
    else break;
  } while (probe < MAX_ADDRESSES);

  return false;

}

uint8_t NetManager::addAddress(uint8_t address)
{
  if (0 == availableAddresses()) return false;

  uint8_t h; 
  uint8_t probe = 0;
  do {
    h = (address + probe) % MAX_ADDRESSES ;
    if (table[h] == address) return false;
    if (table[h]) ++probe;
    else break;
  } while (probe < MAX_ADDRESSES);

  free_addresses--;
  table[h] = address;
  return true;
}
