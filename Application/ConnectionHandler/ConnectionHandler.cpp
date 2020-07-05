#include "ConnectionHandler.h"
#include "ConnectionHandlerTypes.h"

uint8_t ConnectionHandler::buff[] = {0};

ConnectionHandler::ConnectionHandler(Modem & modem, Print & logger)
: fsm::Machine(0, NULL, 0), modem(&modem), logger(&logger), hd(0), tl(0) {}

void ConnectionHandler::handleConnectionStatus(void) {
    if (tl == hd) return;

    uint8_t const signal = buff[hd];
    hd = (--hd) ? hd : sizeof(buff) - 1;

    dispatch(signal);
}

bool ConnectionHandler::sendSignal(uint8_t const signal) {
    if ((tl + 1) == hd || signal > SIGNAL_END) return false;

    tl = (tl + 1) % sizeof(buff);
    buff[tl] = signal;
    return true;
}
