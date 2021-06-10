#include "Scheduler.h"
#include "gtest/gtest.h"

class SchedulerTests : public ::testing::Test {

public:
  static Scheduler::Subscription sub1;
  static Scheduler::Subscription sub2;
  static int a;
  static int b;

  static void updateA(void) { a++; }
  static void updateB(void) { b--; }
};

TEST_F(SchedulerTests, subscribe) {
  Scheduler &scheduler = Scheduler::get();
  scheduler.subscribe(&sub1);

  ASSERT_TRUE(sub1.next == NULL);
  scheduler.subscribe(&sub2);

  ASSERT_TRUE(sub1.next == &sub2);
  ASSERT_TRUE(sub2.next == NULL);

  Scheduler::CheckTime();
  ASSERT_EQ(1, a);
  ASSERT_EQ(-1, b);
}

TEST_F(SchedulerTests, unsubscribe) {
  Scheduler &scheduler = Scheduler::get();
  scheduler.unsubscribe(&sub1);
  scheduler.unsubscribe(&sub2);

  Scheduler::CheckTime();
  ASSERT_EQ(1, a);
  ASSERT_EQ(-1, b);
}

Scheduler::Subscription SchedulerTests::sub1(SchedulerTests::updateA, 1);
Scheduler::Subscription SchedulerTests::sub2(SchedulerTests::updateB, 1);
int SchedulerTests::a = 0;
int SchedulerTests::b = 0;