#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

class Scheduler
{
  public:
  struct Subscription 
  {
    Subscription(void (*subscription)(void), uint16_t const timeout);
    void (*subscription)(void);
    uint16_t timeout;
    Subscription * next;
  };

  private:
  Subscription * subscriptions;
  uint16_t tick;

  public:
  explicit Scheduler(uint16_t const period_ms);

  static Scheduler & get(uint32_t const period_ms);
  static Scheduler & get(void);
  static void CheckTime(void);

  void subscribe(Subscription * const subscription);
  void unsubscribe(Subscription * const subscription);

  private:
  void callSubscriptions(void);

};
#endif /*SCHEDULER_H*/
