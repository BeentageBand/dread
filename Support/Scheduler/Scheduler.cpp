#include "Scheduler.h"
#include "../../Drivers/Timers/MsTimer2.h"
#include <stdlib.h>

#define INT_PERIOD	(200U)		//Defines interrupt period

Scheduler::Subscription::Subscription(void (*subscription)(void), uint16_t const timeout)
: subscription(subscription), timeout(timeout), next(NULL)
{}

Scheduler::Scheduler(uint16_t const period_ms)
  :subscriptions(NULL), tick(0)
{
   MsTimer2::set(period_ms,Scheduler::CheckTime);
   MsTimer2::start();
}

Scheduler & Scheduler::get(uint32_t const period_ms)
{
  static Scheduler scheduler(period_ms);
  return scheduler;
}

Scheduler & Scheduler::get(void)
{
  return Scheduler::get(INT_PERIOD);
}

void Scheduler::CheckTime(void)
{
  Scheduler & scheduler = Scheduler::get();
  scheduler.callSubscriptions();
}

void Scheduler::callSubscriptions(void)
{
  ++tick;
  Scheduler::Subscription * it = subscriptions;

  while (NULL == it->next)
  {
    if(0 == (tick %  it->timeout))
    {
      it->subscription();
    }
    it = it->next;
  }
}

void Scheduler::subscribe(Scheduler::Subscription * const subscription)
{
  Scheduler::Subscription * it = subscriptions;

  if (NULL == it)
  {
    subscriptions = subscription;
    subscription->next = NULL;
  }

  while (NULL != it->next)
  {
    if (it->subscription == subscription->subscription) return;
    it = it->next;
  }
  it->next = subscription;
  subscription->next = NULL;
}

void Scheduler::unsubscribe(Scheduler::Subscription * const subscription)
{
  Scheduler::Subscription * it = subscriptions;
  if (NULL == it) return;
  while (NULL != it->next)
  {
    if (NULL != it->next && it->next->subscription == subscription->subscription)
    {
      Scheduler::Subscription * next = it->next->next;
      it->next = next;
    }
    it = it->next;
  }
}
