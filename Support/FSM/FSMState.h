#ifndef FSM_STATE_H
#define FSM_STATE_H

#include <stdint.h>

#include <avr/pgmspace.h>

#define NO_STATE 255U

namespace fsm {

    class Machine;
    class State;

    struct Transition {

        uint8_t signal;
        uint8_t stid;
        bool (*guard) (Machine &, State &);
        bool (*action) (Machine &);
    };

    class State {

        uint8_t const id;
        uint8_t const ntransition;
        Transition * transition;

        public:
        State(uint8_t const id, Transition * transition, uint8_t const ntransition);
        inline uint8_t get_stid(void) {return id;}
        uint8_t next_state(uint8_t const signal, Transition & transition);
    };
}

#endif // !FSM_STATE_H