#include "gtest/gtest.h"
#include "NetManager.h"

class NetManagerTests : public ::testing::Test {

    public:
    NetManager under_test;

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
