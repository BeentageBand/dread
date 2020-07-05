#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include "Support/Modem/Modem.h"
#include "Support/Persistence/Persistence.h"
#include "Utilities/Print.h"

class DreadClient {

    static bool send_packet;
    Persistence * persistence;
    Modem * modem;
    Print * logger;
    public:
    DreadClient(Persistence & persistence, Modem & modem, Print & logger);

    static void CheckTime(void);
    void uploadFromPersistence(void);

    private:
    
    bool uploadPacket(void);

};
#endif // !WEBCLIENT_H