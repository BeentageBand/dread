#include "FSMState.h"

using namespace fsm;

State::State(uint8_t const id, Transition *transition,
             uint8_t const ntransition)
    : id(id), ntransition(ntransition), transition(transition) {}

uint8_t State::next_state(uint8_t const signal, Transition &out_transition) {
  uint8_t next = NO_STATE;
  for (uint8_t i = 0; i < ntransition; ++i) {
    if (signal == transition[i].signal) {
      out_transition = transition[i];
      break;
    }
  }
  return out_transition.stid;
}
