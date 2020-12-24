/*
 * Dread.h
 *
 * Created: 28/10/2014 11:01:38 a.m.
 *  Author: nedera03
 */ 


#ifndef DREAD_H_
#define DREAD_H_
/*
  DRead Software Revision 1.2
  M12Y2014
  
  Background.
  After several field work, the Master and Slave software version have some bugs. When the network falls, some DReads in the
  network may crash.
  
  DRead Software Revision 1.3
  T21O2014
  
  Background.
  After several field work, the Master has GPRS connection troubleshooting. Some fake GPRS disconnections may happen.
  This version intention is to suppress the software disconnections. GPRS disconnections should happen only if they are
  cellphone provider troubleshooting.
  In addition, the DreadBird program needs new file format. These format includes new elements:
  - Slave remittance of each register 
  
 */
//***************************LIBRARIES**********************************//
//////////////////////////////////////////////////////////////////////////
//Atmega328P
#include "DreadSystem.h"
//Interrupts
#include "MsTimer2.h"
//FAT
#include "SPI.h"
#include "SD.h"
#include "Persistence.h"
//Debug
#include "SoftwareSerial.h"
//GSM
#include <avr/pgmspace.h>
#include "Sim900.h"
////RFID
#include "MifareLite.h"
//RS485
//~~~~
//HID
//~~~~

//**************************DEFINITIONS*********************************//
//////////////////////////////////////////////////////////////////////////
//Interrupts
#define INT_SEC    5    //Defines interrupt duration for one min execution
#define  INT_MINUTES  300    //5 ticks make a second, and 60 seconds a min 60*5=300
// 10:25:39
#define HRS_DEC      date[10] // 1
#define HRS_UNI    date[11] //  0:
#define MIN_DEC    date[13] //    2
#define MIN_UNI    date[14] //     5:
#define SEC_DEC    date[16] //       3
#define SEC_UNI    date[17] //        9
//FAT
#define SD_SS    8    //Slave select for SD
//Debug
#define DBG_RX      3
#define DBG_TX      2
//GSM
#define GSM_NPWD  5
#define GSM_RST   4
#define GSM_BAUD  19200UL
#define GSM_BFRL  16
#define GSM_TOUT  5000UL
#define GSM_MOUT  15000UL
#define FTP_TOUT  20000UL
//RFID
#define MFR_SS    10
#define MFR_NRST   9
//RS485
#define SC_SS    A1
#define NET_MAX    16
#define NT_MASK    0x0F
//HID
#define HID_BUZZ   A2
#define HID_BLD   7
#define HID_RLD    6
//Green-red led
#define HID_GLD   7
//************************FUNC PROTOTYPES*******************************//
//////////////////////////////////////////////////////////////////////////
////INSIDE Functions
void inline beep();
//static void usedAdd(uint8_t *book_ptr);
////INTERRUPTS
void CheckTime();
////SETUP
inline void ConfigSD();
inline void ConfigHID();
inline  void ConfigGPRS();
////LOOP
inline void UploadGPRS();
//inline void net();
inline void SaveRFID2SD();
////MAIN
void setup();
void loop();
//************************FLASH CONSTANTS*******************************//
//////////////////////////////////////////////////////////////////////////

int main(void)
{
  init();
  setup();
    while(1)
    {
    loop();
    }
  return 0;
}

#endif /* DREAD_H_ */
