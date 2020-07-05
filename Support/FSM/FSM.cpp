#include "FSM.h"

using namespace fsm;

Machine::Machine(uint8_t const initial_state, Chart * PROGMEM chart, uint8_t const nstates)
: initial_state(initial_state), current_state(NO_STATE), nstates(nstates), chart(chart) {}

void Machine::dispatch(uint8_t const signal) {

    uint8_t next_state = initial_state;
    Transition transition;

    if (NO_STATE != current_state) {
        State state = get_state(current_state);
        next_state = state.next_state(signal, transition);
    }

    State to_state = get_state(next_state);
    
    if (NO_STATE != to_state.get_stid()) {
        transition(transition, to_state);
    }
}

bool Machine::transition(Transition & transition, State & next_st) {
    if (NULL == transition.guard || transition.guard(*this, next_st)) return false;

    current_state = next_st.get_stid();

    if (NULL == transition.action) return true;

    transition.action(*this);
    return true;
}

State Machine::get_state(uint8_t const state) {
    if (state >= nstates) return State(NO_STATE, NULL, 0);

    Chart * it = chart;
    for (i = 0; i < nstates; ++i) {
        if (chart->state == state) 
            break;
    }

    return State(NO_STATE, chart->transition, chart->ntransitions);
}