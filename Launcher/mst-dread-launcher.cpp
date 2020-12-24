/*
 * RS485_at8MHz_internalCLK_M3M14.cpp
 *
 * Created: 03/03/2014 01:20:26 p.m.
 *  Author: nedera03
 */ 

/*
	DRead Software Revision
	M12Y2014
	
	After several field work, the Master and Slave software version have some bugs. When the network falls, some DReads in the
	network may crash.
	
	
 */
//***************************LIBRARIES**********************************//
//////////////////////////////////////////////////////////////////////////
#include "Dread.h"
#include "ConnectionHandler.h"
#include "DreadClient.h"
#include "NGClient.h"
#include "HID.h"
#include "NetManager.h"
#include "Socket.h"
#include "SC16IS740.h"
#include "FilePersistence.h"
#include "Scheduler.h"
#include "Modem.h"

//************************GLOBAL VARIABLES******************************//
//////////////////////////////////////////////////////////////////////////
//FAT
static File file;
static char CONFIG_file[]="config.txt";   /*config.txt*/
//************************CLASSES******************************//
//////////////////////////////////////////////////////////////////////////
//Interrupts
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
//~~~~
//RS485
static SC16IS740 bus(SC_SS);
static NetManager net_manager;
static Socket socket(bus, SoftSerial);
char const * client_name = "M9X";

static ConnectionHandler connection_handler(modem, SoftSerial);
static DreadClient dread_client(persistence, modem, SoftSerial);
static NGClient ngclient(client_name, socket, net_manager, persistence, SoftSerial);

static Scheduler::Subscription hid_subscription(HID::CheckTime, INT_SEC);
static Scheduler::Subscription connection_subscription(DreadClient::CheckTime, 5 * INT_MINUTES);

////SETUP
//** Function: ConfigSD()
/* Description:
   INLINE function which configures SPI COMM (speed,slave_select) for micro SD card memory usage. Sets the half speed SPI SCK and
   uses pin 8 as SD slave_select. Halts SC16 device by set HIGH SC16 slave_select pin 10.
   Uses HID functions to determine if SD card is inserted, also prevents the program to initialize without SD card.
   The program will start when the micro SD card is inserted properly.
   Variables required:
   - SD			object for SD communication.
   - SoftSerial		SoftSerialging proposals.
 * Notes:
	- Has a saveFileName function, description further but may not be implemented because is redundant and fats the program size up.
	- Setting SC16 SS may be unnecessary, check SC16IS740.cpp to make sure set SC_SS high it's necessary.
	- If the DRead doesn't have a Micro SD card properly inserted, the program will not run further.
	- HID will display card not inserted ERROR.
	- HID config for card inserted ERROR will be defined further.
*/
inline void ConfigSD()
{
	digitalWrite(SC_SS,HIGH);
	while (!SD.begin(SD_SS))
	{
		SoftSerial.write(".");
    HID::get().setLed(BLK_RED);
	}
  HID::get().setLed(BLK_ORG);
}

inline void ConfigHID()
{
   HID::get().setLed(RED_GRN);
}

void setup()
{
  Scheduler::get().subscribe(&hid_subscription);
  Scheduler::get().subscribe(&connection_subscription);

	gprs.begin(GSM_BAUD,GSM_RST,GSM_NPWD);
	SoftSerial.write(client_name);
	//ConfigHID
	ConfigHID();
	//SD
	pinMode(SC_SS,OUTPUT);
	ConfigSD();
	//GSM
	SPI.begin();
	bus.init(SC_1_8432MHz);
	SoftSerial.write("InitNET");
}
void loop()
{
  ngclient.pollRecords();
  connection_handler.handleConnectionStatus();
  connection_handler.listenToModem();
  dread_client.uploadFromPersistence();
}
