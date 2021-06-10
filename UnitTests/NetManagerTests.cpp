#include "gmock/gmock.h"
#include "DreadMock.h"
#include "NetManager.h"
#include "Socket.h"
#include <memory>

class NetManagerTests : public ::testing::Test {

    public:
    NetManager under_test;
};

class SocketTests : public ::testing::Test {
    public:
    std::unique_ptr<Socket> under_test;
    ::testing::NiceMock<PrintMock> port_mock;
    ::testing::NiceMock<PrintMock> logger_mock;

    void SetUp(void) {
        under_test.reset(new Socket(port_mock, logger_mock));
    }

    void expectedLogger(void) {
    }
};

TEST_F(NetManagerTests, emptyNetManagerNoAvailableAddresses) {
    ASSERT_EQ(MAX_ADDRESSES, under_test.availableAddresses());
}

TEST_F(NetManagerTests, doesNotContainAddressReturnFalse) {
    uint8_t address = 46U;
    ASSERT_FALSE(under_test.contains(address));
}

TEST_F(NetManagerTests, containsAddressReturnTrue) {
    uint8_t address = 46U;
    ASSERT_FALSE(under_test.contains(address));
    ASSERT_TRUE(under_test.addAddress(address));
    ASSERT_TRUE(under_test.contains(address));
}

TEST_F(NetManagerTests, when) {
    for (uint8_t i = 0; i < MAX_ADDRESSES; ++i) {
        ASSERT_TRUE(under_test.addAddress(i + 13U));
    }
    ASSERT_EQ(0, under_test.availableAddresses());
}

TEST_F(SocketTests, sendRequest) {
    union Request req;
    EXPECT_CALL(port_mock, write(req.raw, sizeof(req.raw)));
    under_test->send(req);
}

TEST_F(SocketTests, sendResponse) {
    union Response res;
    EXPECT_CALL(port_mock, write(res.raw, sizeof(res.raw)));
    under_test->send(res);
}

TEST_F(SocketTests, processRequest) {
    union Request req;
    expectedLogger();
    under_test->process(req, 10UL);
}

TEST_F(SocketTests, processResponse) {
    union Response res;
    expectedLogger();
    under_test->process(res, 10UL);
}
