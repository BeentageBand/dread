#include "gmock/gmock.h"
#include "DreadMock.h"
#include "Modem.h"
#include <memory>

class ModemTest : public ::testing::Test {
  public:
  std::unique_ptr<Modem> under_test;
  ::testing::NiceMock<PrintMock> port_mock;
  ::testing::NiceMock<PrintMock> logger_mock;


  void SetUp(void) {
    under_test.reset(new Socket(sim900_mock, logger_mock));
  }


};

TEST(ModemTest, isConnected) {
  ASSERT_FALSE(under_test->isConnected());
}
