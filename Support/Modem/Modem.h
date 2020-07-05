#ifndef MODEM_H
#define MODEM_H

#include "Modules/GPRS/Sim900.h"
#include "Modules/SD/SD.h"
#include "Utilities/Print.h"

#define GSM_NPWD  5
#define GSM_RST   4
#define GSM_BAUD  19200UL
#define GSM_BFRL  16
#define GSM_TOUT  5000UL
#define GSM_MOUT  15000UL
#define FTP_TOUT  20000UL

class Modem {

    bool creg_flag;
    File * file;
    Sim900 * sim900;
    Print * logger;

    public:
    Modem(File & file, Sim900 & sim900, Print & logger);

    bool isConnected(void);

    // Upload
    void updatePacketName(char const * filename, uint8_t const fsize);
    bool openPacketSession(void);
    bool sendPacketSize(uint16_t const size);
    void sendPacketData(Print & data_print, uint16_t const data_size);
    bool closePacketSession(void);

    void setRequest(uint8_t const request);
    void setRequestWithData(uint8_t const request, uint8_t const * data, uint8_t const dsize);
    uint8_t getResponse(void);
    uint8_t getResponseWithData(uint8_t * data, uint8_t * dsize);
};

#endif // !MODEM_H
