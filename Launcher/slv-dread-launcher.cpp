/*
 * DreadSlave_V2_0.cpp
 *
 * Created: 12/07/2014 11:44:35 a.m.
 *  Author: nedera03
 */

//////////////////////////////////////////////////////////////////////////
///////////**********************LIBRARIES**********************//////////
#include "CardReader.h"
#include "Dread.h"
#include "HID.h"
#include "HardwareSerial.h"
#include "NGServer.h"
#include "NonPersistence.h"
#include "RS485.h"
#include "Router.h"
#include "Scheduler.h"
#include "Socket.h"
//////////////////////////////////////////////////////////////////////////
///////***********************GlobalVARIABLES**********************///////

#define REG_LIMIT (512U)
//**DEBUG
static SoftwareSerial SoftSerial(DBG_RX, DBG_TX);
//**RFID
static uint8_t Mfr_Key[] = {0x00, 0x00, 0x0B, 0xAC, 0xAF, 0xEA}; // Nedera Key
MifareLite rfid(MFR_SS, MFR_NRST);
static char mfr_book[REG_LIMIT];
static NonPersistence persistence(SoftSerial, mfr_book, REG_LIMIT);
// RS485
static RS485 bus(SC_SS, Serial);
static Socket socket(bus, SoftSerial);
static Router router(socket, SoftSerial);
// AAA:0!
char const *server_name = "S9X";

static CardReader card_reader(rfid, persistence, SoftSerial);
static NGServer ngserver(server_name, router, persistence, SoftSerial);
static Scheduler::Subscription hid_subscription(HID::CheckTime, INT_SEC);
//////////////////////////////////////////////////////////////////////////
////**********************MainFUNCTIONS**********************/////////////
////INSIDE Functions
void inline beep() {
  digitalWrite(HID_BUZZ, HIGH);
  delay(10);
  digitalWrite(HID_BUZZ, LOW);
  delay(10);
  digitalWrite(HID_BUZZ, HIGH);
  delay(10);
  digitalWrite(HID_BUZZ, LOW);
}

//**SETUP
//** Function: ConfigHID()
/* Description:
   INLINE function which configures Port Address for Dual Led and Buzzer. Also
 the function include the namespace MsTimer2 for interruptions. This
 interruptions occurs every second (may be slower) and execute CheckTime void
 function. Variables required:
   - MsTimer2   a namespace that handles Timer 2 interruptions.
 * Notes:
   -
*/
//** Function: ConfigHID()
/* Description:
   INLINE function which configures Port Address for Dual Led and Buzzer. Also
 the function include the namespace MsTimer2 for interruptions. This
 interruptions occurs every second (may be slower) and execute CheckTime void
 function. Variables required:
   - MsTimer2   a namespace that handles Timer 2 interruptions.
 * Notes:
   -
*/
inline void ConfigHID() { HID::get().setLed(RED_GRN); }

///****************************************MAIN*********************************////
void setup() {
  Scheduler::get().subscribe(&hid_subscription);
  /*At this point the whole hardware is conigured to general configuration,
   * preferred by the Arduino open source code.*/
  Serial.begin(19200);        /*Init Serial Driver*/
  SoftSerial.begin(19200);    /*Init Debug Port Serial Driver*/
  pinMode(MFR_SS, OUTPUT);    /*Config MFR522 Pin chip select for SPI*/
  digitalWrite(MFR_SS, HIGH); /*Set pin to HIGH so as to halt MFRC522*/
  ConfigHID();                /*Confing led and buzzer outputs*/

  SPI.begin(); /*Init SPI driver*/
  delay(500);  /*Wait to stabilize SPI*/

  bus.init(); /*init RS485, V 2 uses UART driver*/
  SoftSerial.write("I\n");
  delay(1000);
  rfid.Init(); /*init RFID, uses SPI driver*/
}

void loop() {
  card_reader.ReadAndStore(Mfr_Key, sizeof(Mfr_Key));
  ngserver.listen();
}
