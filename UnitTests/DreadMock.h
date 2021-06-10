#ifndef DREAD_MOCK_H
#define DREAD_MOCK_H
#include "Print.h"
#include "gmock/gmock.h"

class PrintMock : public Print {

public:
  MOCK_METHOD0(read, int(void));
  MOCK_METHOD1(write, void(uint8_t));
  MOCK_METHOD0(available, int(void));
  MOCK_METHOD0(flush, void(void));
  MOCK_METHOD2(write, void(const uint8_t *, uint16_t));
};

#endif // !DREAD_MOCK_H
