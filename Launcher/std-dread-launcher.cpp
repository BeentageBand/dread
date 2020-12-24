/*
 * Standalone_F14M14.cpp
 *
 * Created: 14/03/2014 08:27:28 a.m.
 *  Author: nedera03
 */ 
//***************************LIBRARIES**********************************//
//////////////////////////////////////////////////////////////////////////
#include "Dread.h"
#include "CardReader.h"
#include "ConnectionHandler.h"
#include "DreadClient.h"
#include "HID.h"
#include "FilePersistence.h"
#include "Scheduler.h"
#include "Modem.h"
//************************FLASH CONSTANTS*******************************//
//////////////////////////////////////////////////////////////////////////
/*
//HID
static uint8_t hid_led = LO_ALL;
//~~~~
/************************GLOBAL VARIABLES******************************/
////////////////////////////////////////////////////////////////////////////
//FAT  
static File file;
static char CONFIG_file[]="config.txt";   /*config.txt*/

//************************CLASSES******************************//
//////////////////////////////////////////////////////////////////////////
//Interrupts
//~~~~
//~~~~
//Debug
static SoftwareSerial SoftSerial(DBG_RX,DBG_TX);
//FAT
static uint8_t upload_prefix[4] = "I1Z";
static FilePersistence persistence(file, upload_prefix, SoftSerial);
//GSM
static Sim900 gprs(Serial,SoftSerial);
static Modem modem(file, gprs, SoftSerial);
//RFID
static uint8_t Mfr_Key[]={0x00,0x00,0x0B,0xAC,0xAF,0xEA};  //Nedera Key
static MifareLite rfid(MFR_SS,MFR_NRST);
//RS485
//~~~~
static CardReader card_reader(rfid, persistence, SoftSerial);
static ConnectionHandler connection_handler(modem, SoftSerial);
static DreadClient dread_client(persistence, modem, SoftSerial);

static Scheduler::Subscription hid_subscription(HID::CheckTime, INT_SEC);
static Scheduler::Subscription connection_subscription(DreadClient::CheckTime, 5 * INT_MINUTES);

////SETUP
//** Function: ConfigSD()
/* Description:
   INLINE function which configures SPI COMM (speed,slave_select) for micro SD card memory usage. Sets the half speed SPI SCK and
   uses pin 8 as SD slave_select. Halts RFID device by set HIGH Mifare slave_select pin 10.
   Uses HID functions to determine if SD card is inserted, also prevents the program to initialize without SD card.
   The program will start when the micro SD card is inserted properly.
   Variables required:
   - SD         object for SD communication.
   - SoftSerial      debugging proposals.
 * Notes:
   - Has a saveFileName function, description further but may not be implemented because is redundant and fats the program size up.
   - Setting RFID SS may be unnecessary, check MifareLite.cpp to make sure set MFR_SS high it's necessary.
   - If the DRead doesn't have a Micro SD card properly inserted, the program will not run further.
   - HID will display card not inserted ERROR.
   - HID config for card inserted ERROR will be defined further.
*/
inline void ConfigSD()
{
   digitalWrite(MFR_SS,HIGH);
   while (!SD.begin(SD_SS))
   {
      SoftSerial.write('.');
      HID::get().setLed(BLK_RED);
   }
   HID::get().setLed(BLK_ORG);
   //readFileName();
}
//** Function: ConfigHID()
/* Description:
   INLINE function which configures Port Address for Dual Led and Buzzer. Also the function include the namespace MsTimer2 for interruptions.
   This interruptions occurs every second (may be slower) and execute CheckTime void function.
   Variables required:
   - MsTimer2   a namespace that handles Timer 2 interruptions.
 * Notes:
   -
*/
inline void ConfigHID()
{
   HID::get().setLed(RED_GRN);
}

inline void ConfigGPRS()
{
   /*Variable to store return value of gprs methods.*/
  /*Wait for the modem to send "*PSUTTZ..".
  Clock will be sync an ready to read RFID*/

  /*If modem do not reg to network, at least PSUTTZ,
  RESET modem and repeat the sequence.*/
  /*FTP_TOUT shift left to multiply by 4 (shift left 2).*/
  /*Blinks green led if was able to register to network. Blinks red led if not.*/
  /*Reads config.txt inside SD. Firs line is +CGREG: 1,*/
  /*Waits for modem to send "+CGREG: 1,1".
  Determines if modem was able to be registered to the Network.*/
  /*Blinks green led if  not able to register to network.*/
  /*Blinks green led if was able to register to network.*/

/*  Deprecated in V 1.3
   Reasons: The config file was executed to set the SIM900 modem parameter in order to use the GPRS with APN account.
   There was a problem with this structure, the configuration is Service Provider dependent and it was done once (during setup).
   Therefore, if Service Provider wasn't found, there was no retry on configuration parameter.
   Even if the GPRS was available,   Sim900 modem couldn't achieve a successful connection.
   DownloadGPRS has only to set parameter found in config.txt, so config.txt was mixed to ID.txt, helping the spot of
*/ 
/*
   buffer[0]='\"';
   for(counter=0;counter<7;counter++)
   {
      buffer[counter+1]=file_download[counter];
      SoftSerial.println(gsm_retry,DEC);
   }
   SoftSerial.println(gsm_retry,DEC);
   buffer[++counter]='\"';
   gprs.setAtParameter(FTP_GET_Filename,OK,buffer,counter+1,GSM_TOUT);//send the filename command with file_download parameter
 */

   ////config.txt will get the last saved PUT filename from SIM900m
   /*AT+FTPSCONT?*/
   /*+FTPPUTNAME: "*/
  /*Reads last saved file_name from modem*/
  /*Modem sends extra OK after sending ftpputname*/
   //SoftSerial.write((uint8_t*)file_name,sizeof(file_name)-1);
   //SoftSerial.println();
   //Update Date
   /*"AT+CCLK?"*/
   /*"+CCLK: */

   /*change SIM900 CCLK format = "date,hour"
   to "date   hour"*/
   /*Modem sends extra OK after sending cclk*/
   //SoftSerial.write(date,sizeof(date)-1);
   //SoftSerial.println();
   //Read config.txt file to execute the config sequence.
}

void setup()
{

  Scheduler::get().subscribe(&hid_subscription);
  Scheduler::get().subscribe(&connection_subscription);

   //Init Serials GPRS and debug
   gprs.begin(GSM_BAUD,GSM_RST,GSM_NPWD);
   //ConfigHID
   ConfigHID();
   //SD
   pinMode(SC_SS,OUTPUT);
   ConfigSD();
   //GSM
   ConfigGPRS();
   SPI.end();
   delay(1000);
   SPI.begin();
   rfid.Init();
   //Set last serial to 0's

   SoftSerial.write('R');            /*Print "RFID"*/
   SoftSerial.write('F');
   SoftSerial.write('I');
   SoftSerial.write('D');
   SoftSerial.println();
}
void loop()
{
  card_reader.ReadAndStore(Mfr_Key, sizeof(Mfr_Key));
  connection_handler.handleConnectionStatus();
  connection_handler.listenToModem();
  dread_client.uploadFromPersistence();
}
