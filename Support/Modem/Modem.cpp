#include "Modem.h"

//RESPONSES
static const uint8_t plusCGREG[] PROGMEM=            "CGREG: 1,\r";
static const uint8_t PSUTTZ[] PROGMEM=               "*PSUTTZ\r";
static const uint8_t OK[] PROGMEM=                  "OK\r";
static const uint8_t plusCCLK[] PROGMEM=            "CCLK: \r";
static const uint8_t plusFTP_PUT[] PROGMEM =         "FTPPUT:1,1,\r";// next goes lengthLENGTH
static const uint8_t plusFTP_PUT_Closed[] PROGMEM=      "FTPPUT:1,0\r";
static const uint8_t plusFTP_PUT_Length[] PROGMEM=      "FTPPUT:2,\r";
//*Commands
const uint8_t FTPSCONT[] PROGMEM=            "FTPSCONT\r";
const uint8_t RESET[] PROGMEM=               "CFUN=1,1\r";
static const uint8_t readCCLK[] PROGMEM=            "CCLK?\r";
static const uint8_t FTP_PUT_Filename[] PROGMEM=      "FTPPUTNAME=\r";
static const uint8_t FTP_PUT_Open[] PROGMEM=         "FTPPUT=1\r";
static const uint8_t FTP_PUT_Length[] PROGMEM=         "FTPPUT=2,\r";
static const uint8_t FTP_PUT_Close[] PROGMEM=         "FTPPUT=2,0\r";

Modem::Modem(File & file, Sim900 & sim900, Print & logger)
: file(&file), sim900(&sim900), logger(&logger) {}

bool Modem::isConnected(void) { 
  bool cgreg_flag = false;
  return cgreg_flag;
}

bool Modem::closePacketSession(void) {
   sim900->setAtCmd(FTP_PUT_Close,OK,GSM_TOUT);
   sim900->waitFor(plusFTP_PUT_Closed,GSM_MOUT);
}

void Modem::updatePacketName(char const * filename, uint8_t const fsize) {
    sim900->setAtParameter(FTP_PUT_Filename,OK, (uint8_t *)filename,fsize,GSM_TOUT);   /*Sends the filename command with file_name copied on buffer*/
}

bool Modem::openPacketSession(void) {
  sim900->setAtCmd(FTP_PUT_Open,OK,GSM_TOUT);   /*Open a PUT session*/
  uint8_t gsm_pointer = 0;
  return sim900->matchResponse(plusFTP_PUT, gsm_pointer);
}

bool Modem::sendPacketSize(uint16_t const size) {

}

void Modem::sendPacketData(Print & data_print, uint16_t const data_size) {

    bool result = sim900->setAtParameter(FTP_PUT_Length,plusFTP_PUT_Length,
                              data_size,
                              GSM_MOUT);                     /*Set how many bytes will be transfer*/
    logger->println();
    //Uploading registers*******************************************
    uint16_t n = data_size;
    while(result && n--)                                       /*Send contents on file_name.*/
    {
        sim900->getPrint().write(data_print.read());
    }
    //After sending stream, wait for OK
    sim900->waitFor(OK,GSM_TOUT) && sim900->waitFor(plusFTP_PUT,GSM_TOUT);
}
