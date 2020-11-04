#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include "Support/Modem/Modem.h"
#include "Support/Persistence/FilePersistence.h"
#include "Utilities/Print.h"

class DreadClient {

    static bool send_packet;
    FilePersistence * persistence;
    Modem * modem;
    Print * logger;
    public:
    DreadClient(FilePersistence & persistence, Modem & modem, Print & logger);

    static void CheckTime(void);
    void uploadFromPersistence(void);

    private:
    
    bool uploadPacket(void);

};
#endif // !WEBCLIENT_H
