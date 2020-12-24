#ifndef FSM_H
#define FSM_H

#include "FSMState.h"
#include "DreadConfig.h"

namespace fsm {


    struct Chart {
        uint8_t state;
        Transition * transition;
        uint8_t ntransitions;
    };

    class Machine {

        uint8_t const initial_state;
        uint8_t const nstates;
        uint8_t current_state;
        Chart * chart;

        public:
        Machine(uint8_t const initial_state, Chart * chart, uint8_t const nstates);

        void dispatch(uint8_t const signal);
        private:
        bool transition(Transition & transition, State const & next_st);
        State get_state(uint8_t const state);
    };

}

#endif // !FSM_H
