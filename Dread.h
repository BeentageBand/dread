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
//Watchdog Timer
//#include <avr/wdt.h>
//Atmega328P
#include "Utilities/DreadSystem.h"
//Interrupts
#include "Drivers/Timers/MsTimer2.h"
//FAT
#include "Drivers/SPI/SPI.h"
#include "Modules/SD/SD.h"
//Debug
#include "Drivers/SoftwareSerial/SoftwareSerial.h"
//GSM
#include <avr/pgmspace.h>
#include "Modules/GPRS/Sim900.h"
////RFID
#include "Modules/RFID/MifareLite.h"
//RS485
//~~~~
//HID
//~~~~

//**************************DEFINITIONS*********************************//
//////////////////////////////////////////////////////////////////////////
//Interrupts
#define INT_PERIOD	200		//Defines interrupt period
#define INT_SEC		5		//Defines interrupt duration for one min execution
#define	INT_MINUTES	300		//5 ticks make a second, and 60 seconds a min 60*5=300
// 10:25:39
#define HRS_DEC    	date[10] // 1
#define HRS_UNI		date[11] //  0:
#define MIN_DEC		date[13] //    2
#define MIN_UNI		date[14] //     5:
#define SEC_DEC		date[16] //       3
#define SEC_UNI		date[17] //        9
//FAT
#define SD_SS		8		//Slave select for SD
//Debug
#define DBG_RX      3
#define DBG_TX      2
//GSM
#define GSM_NPWD	5
#define GSM_RST 	4
#define GSM_BAUD	19200UL
#define GSM_BFRL	16
#define GSM_TOUT	5000UL
#define GSM_MOUT	15000UL
#define FTP_TOUT	20000UL
//RFID
#define MFR_SS		10
#define MFR_NRST 	9
//RS485
#define SC_SS		A1
#define NET_MAX		16
#define NT_MASK		0x0F
//HID
#define HID_BUZZ 	A2
#define HID_BLD 	7
#define HID_RLD  	6
#define LO_ALL		0x00
#define HI_RED		0x01
#define HI_BLU		0x02
#define HI_ORG		0x03
#define BLK_RED		0x05
#define VLT_BLU		0x07
#define BLK_BLU		0x0A
#define VLT_RED		0x0B
#define RED_BLU		0x0D
#define BLK_VLT		0x0F
//Green-red led
#define HID_GLD 	7
#define HI_GRN		0x02
#define HI_ORG		0x03
#define BLK_RED		0x05
#define ORG_GRN		0x07
#define BLK_GRN		0x0A
#define ORG_RED		0x0B
#define RED_GRN		0x0D
#define BLK_ORG		0x0F
//************************FUNC PROTOTYPES*******************************//
//////////////////////////////////////////////////////////////////////////
////INSIDE Functions
void inline beep();
//static void usedAdd(uint8_t *book_ptr);
static void setLed(uint8_t led_setup);
static boolean nextLine(const char*filename);
uint32_t PrintLong(const uint8_t*str,uint8_t ptr);
boolean equalStr(const uint8_t*str1,const uint8_t*str2);
inline boolean DumpFile();
inline void generateFileNameIUD();
inline boolean uploadPacket();
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
