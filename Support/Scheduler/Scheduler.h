#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

class Scheduler {
public:
  struct Subscription {
    Subscription(void (*subscription)(void), uint16_t const timeout);
    void (*subscription)(void);
    uint16_t timeout;
    Subscription *next;
  };

  enum { INT_PERIOD = 200U };

private:
  Subscription *subscriptions;
  uint16_t tick;

  Scheduler(void);

public:
  static Scheduler &get(void);
  static void CheckTime(void);

  void subscribe(Subscription *const subscription);
  void unsubscribe(Subscription *const subscription);

private:
  void callSubscriptions(void);
};
#endif /*SCHEDULER_H*/
