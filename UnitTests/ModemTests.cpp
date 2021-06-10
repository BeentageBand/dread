#include "DreadMock.h"
#include "Modem.h"
#include "gmock/gmock.h"
#include <memory>

class ModemTest : public ::testing::Test {
public:
  std::unique_ptr<Modem> under_test;
  ::testing::NiceMock<PrintMock> port_mock;
  ::testing::NiceMock<PrintMock> logger_mock;
  std::unique_ptr<Sim900> sim900;

  void SetUp(void) { 
    sim900.reset(new Sim900(port_mock, logger_mock));
    under_test.reset(new Modem(*sim900, logger_mock)); 
  }
};

TEST_F(ModemTest, isConnected) { ASSERT_FALSE(under_test->isConnected()); }
