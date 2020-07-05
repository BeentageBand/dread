#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Support/FSM/FSM.h"
#include "Support/Modem/Modem.h"
#include "Utilities/Print.h"

class ConnectionHandler : public fsm::Machine {

    static uint8_t buff[4];
    Modem * modem;
    Print * logger;
    uint8_t hd;
    uint8_t tl;

    public:
    ConnectionHandler(Modem & modem,Print & logger);

    void handleConnectionStatus(void);
    bool sendSignal(uint8_t const signal);

    private:

};

#endif // !CONNECTION_HANDLER_H