#include "Scheduler.h"
#include <stdlib.h>

Scheduler::Subscription::Subscription(void (*subscription)(void), uint16_t const timeout)
: subscription(subscription), timeout(timeout), next(NULL)
{}

Scheduler::Scheduler(void)
  :subscriptions(NULL), tick(0)
{
}

Scheduler & Scheduler::get(void)
{
  static Scheduler scheduler;
  return scheduler;
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

  while (NULL != it)
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

  if (NULL == subscriptions)
  {
    subscriptions = subscription;
    subscription->next = NULL;
    return;
  }

  Scheduler::Subscription * it = subscriptions;

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
  if (NULL == subscriptions) return;

  Scheduler::Subscription prev(NULL, 0);
  prev.next = subscriptions;

  Scheduler::Subscription * it = &prev;
  while(NULL != it->next) {
    if (it->next == subscription) {
      it->next = it->next->next;
    } else {
      it = it->next;
    }
  }
  subscriptions = prev.next;
}
