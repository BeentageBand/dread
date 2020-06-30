/*
 * Standalone_F14M14.cpp
 *
 * Created: 14/03/2014 08:27:28 a.m.
 *  Author: nedera03
 */ 
//***************************LIBRARIES**********************************//
//////////////////////////////////////////////////////////////////////////
#include "Dread.h"
//************************FLASH CONSTANTS*******************************//
//////////////////////////////////////////////////////////////////////////
/*
typedef enum
{
   READ_CCLK_RESTART, WAIT_CGREG, SEND_AT_FROM_FILE,
   WAIT_AT_FROM_FILE,
   }gprs_state_t;*/
//Interrupts
//~~~~
//******GSM
//RESPONSES
const uint8_t plusCGREG[] PROGMEM=            "CGREG: 1,\r";
const uint8_t PSUTTZ[] PROGMEM=               "*PSUTTZ\r";
const uint8_t OK[] PROGMEM=                  "OK\r";
const uint8_t plusCCLK[] PROGMEM=            "CCLK: \r";
//const uint8_t plusFTP_PUT_Filename[] PROGMEM=   "FTPPUTNAME: \"\r";
const uint8_t plusFTP_PUT[] PROGMEM =         "FTPPUT:1,1,\r";// next goes lengthLENGTH
const uint8_t plusFTP_PUT_Closed[] PROGMEM=      "FTPPUT:1,0\r";
const uint8_t plusFTP_PUT_Length[] PROGMEM=      "FTPPUT:2,\r";
//const uint8_t plusFTP_GET_Closed[] PROGMEM=   "FTPGET:1,0\r";
//const uint8_t plusFTP_GET_Available[] PROGMEM=   "FTPGET:1,1\r";
//const uint8_t plusFTP_GET_Length[] PROGMEM=   "FTPGET:2,\r";
//*Commands
//const uint8_t FTP_GET_Filename[] PROGMEM=      "FTPGETNAME=\r";
const uint8_t FTPSCONT[] PROGMEM=            "FTPSCONT\r";
const uint8_t RESET[] PROGMEM=               "CFUN=1,1\r";
//const uint8_t readFTPSCONT[] PROGMEM=         "FTPSCONT?\r";
const uint8_t readCCLK[] PROGMEM=            "CCLK?\r";
const uint8_t FTP_PUT_Filename[] PROGMEM=      "FTPPUTNAME=\r";
const uint8_t FTP_PUT_Open[] PROGMEM=         "FTPPUT=1\r";
const uint8_t FTP_PUT_Length[] PROGMEM=         "FTPPUT=2,\r";
const uint8_t FTP_PUT_Close[] PROGMEM=         "FTPPUT=2,0\r";
//const uint8_t FTP_GET_Open[] PROGMEM=         "FTPGET=1\r";
//const uint8_t FTP_GET_Length[] PROGMEM=      "FTPGET=2,\r";
////RFID
//~~~~
//RS485
//~~~~
//HID
//~~~~
//************************GLOBAL VARIABLES******************************//
////////////////////////////////////////////////////////////////////////////
static uint8_t buffer[16];
//Interrupts
static uint8_t date[23];         /*Stores CCLK: "14/02/14,13:45:08-28"*/
/*0123456789012345678901*/
uint16_t tick=0;

//FAT  
File file;
char file_download[]="I1Z.txt";
char file_name[13];               /*Stores FTPPUTNAME: "P1I006TC.txt"*/
/*012345678901*/
static uint8_t file_line[53];
uint16_t  file_ptr;
char CONFIG_file[]="config.txt";   /*config.txt*/
//GSM
uint8_t gsm_task;
uint8_t gsm_pointer;
uint32_t gsm_timer;

//RFID
static uint8_t mfr_serial[5];
static uint8_t mfr_lastSer[5];
static uint8_t Mfr_Key[]={0x00,0x00,0x0B,0xAC,0xAF,0xEA};  //Nedera Key

//RS485
/*
uint8_t add_book[NET_MAX][3];
uint8_t add_ptr;
uint8_t reg_try;*/

//HID
boolean   red;
boolean   green;
boolean blink_red;
boolean blink_grn;
boolean SET_LED=true;
uint8_t hid_led;
//************************CLASSES******************************//
//////////////////////////////////////////////////////////////////////////
//Interrupts
//~~~~
//FAT
//~~~~
//Debug
SoftwareSerial SoftSerial(DBG_RX,DBG_TX);
//GSM
Sim900 gprs(Serial,SoftSerial);
//RFID
MifareLite rfid(MFR_SS,MFR_NRST);
//RS485
//~~~~
//HID
//~~~~
//////////////////////////////////////////////////////////////////////////
////**********************MainFUNCTIONS**********************/////////////
////INSIDE Functions
void inline beep(){
   digitalWrite(HID_BUZZ,HIGH);
   delay(10);
   digitalWrite(HID_BUZZ,LOW);
   delay(10);
   digitalWrite(HID_BUZZ,HIGH);
   delay(10);
   digitalWrite(HID_BUZZ,LOW);

}
//** Function: setLed(char led_setup)
/* Description:
   This function makes HID code "easy" to read or debug. Handles 16 led behaviors, all of them are 4 bit chars. The behaviors are defined
   up in the definition code in this cpp file.
   Variables required:
   - red      state of the HID_RLD pin.
   - green      the unsigned long generated. the function return id.
   - blink_red   controls the red led blink (not blink)
   - blink_grn   controls the green led blink (not blink)
   - led_setup  used to configure led behavior.
   * Notes:
   - pins need to be set as OUTPUT.
   - The blink frequency is determined by INT_PERIOD, and the timer interrupt should have a routine write this behavior to the portpin outputs.
   - Dual led can generate 3 colors: red, orange and green, each with blinking and toggling between colors.
   - A buzzer pin behavior may be implemented as well.

*/
inline void setLed(uint8_t led_setup)
{
   SET_LED=false;

   blink_grn=blink_red=red=green=false; /*Set the outputs to low*/

   red=led_setup&0x01;
   led_setup=led_setup>>1;

   green=led_setup&0x01;
   led_setup=led_setup>>1;

   blink_red=led_setup&0x01;
   led_setup=led_setup>>1;

   blink_grn=led_setup&0x01;

   SET_LED=true;
}

static boolean nextLine(const char*filename)
{
   boolean file_end=false;

   SD.begin(SD_SS);                        /*Initiate SD*/
   //Open the file with name config.txt or DReadID.txt
   //check if the method will find next line??
   file=SD.open(filename,FILE_READ);
   if(file)
   {
      uint8_t in_byte;
      uint8_t* line_cpy=file_line;
      file.seek(file_ptr);
      do
      {
         file_end=file.available()<=0;         /*Check if there's data on file.*/
         if(!file_end)
         {
            in_byte=file.read();
            *line_cpy=in_byte;
            ++line_cpy;
         }
      } while (!file_end && in_byte!='\r');
   }
   else
   {
      SoftSerial.write('U');
      SoftSerial.write('n');
      SoftSerial.println();
   }
   file_ptr=file.position();
   file.close();

   return file_end;
}
//** Function: PrintLong(unsigned char *str,char ptr)
/* Description:
   The function takes char array and concatenate 5 consecutive chars into a unsigned long. The print function can handle unsigned long prints.
   Variables required:
   - str      the array where the 5 chars are going to be taken.
   - id         the unsigned long generated. the function return id.
   - ptr      for array shifting functions.
   * Notes:
   - Nedera saves long values in S150 Mifare Cards. Longs datatypes can have only 4 bytes. 
   - 5 bytes are necessary because the serial Num of each S150 Mifare Card is written in the text file as well.

*/
uint32_t PrintLong(const uint8_t *str,uint8_t ptr){
  uint32_t id=0;
    id|=(uint8_t)str[ptr++];
    id=id<<8;
   id|=(uint8_t)str[ptr++];
    id=id<<8;
    id|=(uint8_t)str[ptr++];
    id=id<<8;
    id|=(uint8_t)str[ptr++];
    id=id<<8;
    id|=(uint8_t)str[ptr++];
   return id;
}
boolean equalStr(const uint8_t*str1,const uint8_t*str2)
{
   uint8_t ptr;
   boolean result=false;
      ptr=0;
      do
      {
         result=str1[ptr]==str2[ptr];
         ++ptr;
      } while (ptr<5&&result);

   return result;
}
//** Function: DumpFile()
/* Description:
   The method removes the File from the SD card. After that, generates a new file_name in order to
   continue reading S150 Mifare Cards. 
   Checks if file exist to remove it, if not it only switches to RFID COMM.
   Generates a new file_name using generateFileNameIUD() function after the las file was succesfully removed.
   Variables required:
   - SoftSerial       A COM port for debugging purposes. It bulks program size, so the final version of the code this methods are
            going to be commented (not compiled);
   - SD       communicates with SD. For creating/opening/dumping files.
   - file       object which uses FAT16 protocol for file management. Writes or read files.
   - rfid      object that switches to RFID SPI comm after DumpFile is done.
   relevant and developed in FTPDownloadGPRS programs.
 * Notes:
   - SoftSerial strings were write in short pseudo code, reduce program size.
   - saveFileName() may be not necessary,
   - if readFileName() is commented, be sure saveFileName() is commented too, if not it will cause program problems because code is still
     compilable.
   - the SD.exist is just for error issues, the confirmation may not be necessary because DumpFile is a function inside FTPUploadGPRS()
*/
boolean inline DumpFile(){
   SoftSerial.write('R');               /*Write on debug "R*\r"*/
   SoftSerial.write('*');
   SoftSerial.println();

   SD.remove(file_name);

   if (!SD.exists(file_name))            /*Removes the file with file_name. Check if exist to validate
                                 removal. Non existent = removed, existent =not removed */
   {
      //SoftSerial.println("genF*");
      generateFileNameIUD();

      SoftSerial.write('D');
      SoftSerial.println();
      hid_led=LO_ALL;
      setLed(hid_led);
      return true;
   }
   else
   {
      SoftSerial.write('U');
      SoftSerial.write('*');
      SoftSerial.println();
      hid_led=BLK_RED;
      setLed(hid_led);
      return false;
   }

   SPI.end();
   SPI.begin();
   rfid.Init();                     /*Reboot RFID*/
}
//** Function: generateFileNameIUD()
/* Description:
   The function modifies the file_name array and generates a "new" file_name for the up loadable file. The reason is because if DRead upload a
   file with the same name as one in the server, the file will be replaced, that may cause a data loses.
   The filename is going to be generated by the following parameters: D-ReadID+FileUID+".txt".
   > D-ReadID: is the Identifier of the D-Read, compound by 3 alphanumeric characters (No case sensitive). Each D-Read has its own.
   > FileUID: this ID is generated for unique file, the intention is to not overwrite files in the server. Compound by 5 alphanumeric characters
   (No case sensitive), the D-Read can generate 1679616 unique files. If D-Read generate a new file every minutes, D-Read would overflow the UID
   until 7 years later!!, considering that D-Read is saving RFID tags all the time, which is not the case and UID generation can last even longer.
   Variables required:
   - file_name      Is modified during the process.
   - ptr         local val which shifts << the file_name array. the char 3 to 7 of the array are the 5 UID chars.
 * Notes:
   - IUD has 5 alphanumeric characters, it will permute 36^5 different forms.

*/
inline void generateFileNameIUD()
{
   uint8_t counter=7U;                     /*Pointer will go backwards.File_name only changes first 8 characters [0-7]*/
   do
   {
      switch (file_name[counter])
      {
         case 'Z':                     /*'Z' to '0'. Change previous character."AZ" to "B0"*/
         file_name[counter]='0';
         --counter;
         break;
         case '9':                     /*'9' to 'A'."A9" to "AA". Finish*/
         file_name[counter]='A';
         counter=2U;
         break;
         default:
         ++file_name[counter];            /*Adds 1 to char, ASCII have alphanumeric order. Finish*/
         counter=2U;
         break;
      }

   } while (file_name[counter+1]=='0'
   && counter>2U);               /*Checks char overflow to continue with file generation, up to 10*/
}
inline boolean uploadPacket()
{
   uint32_t file_length=0;               /*Variable that stores the backup file size. Up to 2^31*/
   uint8_t  list_length=0;               /*Variable that stores the slave book size. This is list is uploaded when upload is successful*/
   boolean result;                     /*Stores the return value of gprs methods.*/

   SD.begin(SD_SS);                  /*Start SD. No need to validate it once it started*/
   file=SD.open(file_name,FILE_READ);      /*Open upload file*/
   file_length=file.size();            /*Get the file full size. Returns 0 if not file*/

   //SoftSerial.printNumber(file_length,DEC);                  /*Prints file_length, it will decrease with each packet.*/
   //SoftSerial.println();

   list_length+=19U;                  /*add length of date + \r: 19 bytes*/

   result=true;                     /*Preset result as true. It will turn false if modem instructions fails
                                 and it will cancel the uploading.*/
   while((uint32_t)(file_length+(uint32_t)list_length)>0UL && result)
   {
      if(file_length>500UL)
      {
         uint16_t counter=500U;

         result=gprs.setAtParameter(FTP_PUT_Length,plusFTP_PUT_Length,
                              counter+(uint16_t)list_length,
                              GSM_MOUT);                     /*Set how many bytes will be transfer*/

         if (list_length>0)                                    /*Checks if slaves and timestamp were already sent.
                                                         List_length>0 if slaves and timestamp haven't been sent.
                                                         List_length=0 if slaves and timestamp were sent.*/
         {
            //Sending timestamp*****************************************
            for (uint8_t i=1;i<18U;++i)                           /*Sends timestamp at the very first line*/
            {
               Serial.write(date[i]);
            }
            Serial.println();


            list_length=0;                                    /*Once timestamp was sent, list_length is 0*/
         }

         SoftSerial.printNumber(file_length,DEC);                  /*Prints file_length, it will decrease with each packet.*/
         SoftSerial.println();
         //Uploading registers*******************************************
         while(counter)                                       /*Send contents on file_name.*/
         {
            Serial.write(file.read());
            --counter;
         }
         file_length-=500UL;                                    /*Substract 500 from size*/
      }
      else
      {
         SoftSerial.printNumber(file_length,DEC);
         SoftSerial.println();
         result=gprs.setAtParameter(FTP_PUT_Length,FTP_PUT_Length,
                             (uint16_t)file_length+
                             (uint16_t)list_length,            /*Set how many bytes will be transfer*/
                             GSM_MOUT);
         if (list_length>0)                                    /*Checks if slaves and timestamp were already sent.
                                                         List_length>0 if slaves and timestamp haven't been sent.
                                                         List_length=0 if slaves and timestamp were sent.*/
         {
            //Sending timestamp*****************************************
            for (uint8_t i=1;i<18;++i)                           /*Sends timestamp at the very first line*/
            {
               Serial.write(date[i]);
            }
            Serial.println();

            list_length=0;                                    /*Once timestamp was sent, list_length is 0*/
         }
         while(file_length)
         {
            Serial.write(file.read());
            --file_length;
         }

      }
      gprs.waitFor(OK,GSM_TOUT);//After sending stream, wait for OK
      gprs.waitFor(plusFTP_PUT,GSM_TOUT);
   }
   file.close();

   SoftSerial.write('D');
   SoftSerial.println();
   gprs.setAtCmd(FTP_PUT_Close,OK,GSM_TOUT);
   gprs.waitFor(plusFTP_PUT_Closed,GSM_MOUT);
   //Uploading was successful when the file was totally sended
   return file_length==0 && result;
}
////INTERRUPTS
//** Function: CheckTime()
/* Description:
   This function is called by hardware interruptions every second (hardcoded). This functions implements the following tasks:
   - Generate a TICK counter which manage other TICK events in the program.
   - Checks UPLOAD flags (once per minute) on TICK event.
   - Manage HID side functions like leds' blinking TICK events.
   Variables required:
   - HID   the whole set of HID led variables.
   - Tick   this counter generate ticks for multitasking.
 * Notes:
   - Multi task may be implemented, can help to avoid time delays in the program. However, the multi task implementation can change the whole
     program structure heavily.
   - Timer Ticks have 1 second period, this may set down for multitasking. In addition, HID led blinking can be quicker, more versatile, more
     friendly (transmitting blinks and standby blinks).
   -
*/
void CheckTime()
{
   //Blinking Led method
   //if blink methods are HIGH, led s toggles, else the led s don't change.
   if(SET_LED)
   {
      red=red^blink_red;
      green=green^blink_grn;
      digitalWrite(HID_RLD,red);
      digitalWrite(HID_GLD,green);
   }

   //adds one second to the counter.
   ++tick;

   if((tick%INT_SEC==0))
   {
      //SoftSerial.println(tick,DEC);
       if (SEC_UNI>='9')
       {
          SEC_UNI='0'; //Seq X9" -> X0"

          if (SEC_DEC>='5')
          {
             SEC_DEC='0'; //59" -> 00"
          }
          else
          {
             SEC_DEC++;  //09"->10"
          }
       }
       else
       {
          SEC_UNI++;//00"->01"
       }
   }
   //Change hour:minutes if the seconds overflows 60 counts.
   if(tick>=INT_MINUTES)///////////////////CHANGE to 60 further. 20 is for debugging purposes!!!
   {
      //Use if methods, may use less memory than math.
      //Every 60 seconds, add a minute and seconds resets.
      //minutes++;
      tick=0;
      file_ptr=0;
      ////Every minute the UPLOAD method is available.
      gsm_task=0;
   }
}
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
      setLed(BLK_RED);
   }
   setLed(BLK_ORG);
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
   pinMode(HID_RLD,OUTPUT);
   pinMode(HID_GLD,OUTPUT);
   pinMode(HID_BUZZ,OUTPUT);
   digitalWrite(HID_RLD,LOW);
   digitalWrite(HID_GLD,LOW);
   digitalWrite(HID_BUZZ,LOW);
   //Interrupts
   MsTimer2::set(INT_PERIOD,CheckTime);
   MsTimer2::start();
   hid_led=RED_GRN;
   setLed(hid_led);
}
inline void ConfigGPRS()
{
   boolean result=false;                        /*Variable to store return value of gprs methods.*/
   uint8_t response_ptr=0;
   uint32_t time=millis();

   file_ptr=0;
   while(!result)
   {
      result=gprs.matchResponse(PSUTTZ,&response_ptr);/*Wait for the modem to send "*PSUTTZ..".
                                          Clock will be sync an ready to read RFID*/

      if(!result &&                           /*If modem do not reg to network, at least PSUTTZ,
                                          RESET modem and repeat the sequence.*/
      (millis()-time)>(uint32_t)(GSM_MOUT<<2))      /*FTP_TOUT shift left to multiply by 4 (shift left 2).*/
      {
         gprs.ATCmd(RESET);
         time=millis();
      }
   }

   /*Blinks green led if was able to register to network. Blinks red led if not.*/
   nextLine(CONFIG_file);                        /*Reads config.txt inside SD. Firs line is +CGREG: 1,*/
   result=gprs.waitForC(file_line,GSM_TOUT);         /*Waits for modem to send "+CGREG: 1,1".
                                          Determines if modem was able to be registered to the Network.*/
   if (!result)
   {
      setLed(BLK_RED);                        /*Blinks green led if  not able to register to network.*/
   }
   else
   {
      setLed(BLK_GRN);                        /*Blinks green led if was able to register to network.*/
   }

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
   nextLine(CONFIG_file);
   gprs.ATCmdC(file_line);                        /*AT+FTPSCONT?*/
   nextLine(CONFIG_file);
   gprs.waitForC(file_line,GSM_TOUT);               /*+FTPPUTNAME: "*/

   gprs.receiveStream((uint8_t*)file_name,
                  sizeof(file_name)-1);            /*Reads last saved file_name from modem*/
   gprs.waitFor(OK,GSM_TOUT);                     /*Modem sends extra OK after sending ftpputname*/
   //SoftSerial.write((uint8_t*)file_name,sizeof(file_name)-1);
   //SoftSerial.println();
   //Update Date
   nextLine(CONFIG_file);
   gprs.ATCmdC(file_line);                        /*"AT+CCLK?"*/
   nextLine(CONFIG_file);
   gprs.waitForC(file_line,GSM_MOUT);               /*"+CCLK: */

   gprs.receiveStream(date,sizeof(date)-1);
   date[9]='\t';                              /*change SIM900 CCLK format = "date,hour"
                                          to "date   hour"*/
   gprs.waitFor(OK,GSM_TOUT);                     /*Modem sends extra OK after sending cclk*/
   //SoftSerial.write(date,sizeof(date)-1);
   //SoftSerial.println();
   //Read config.txt file to execute the config sequence.
   do
   {
      result=nextLine(CONFIG_file);

      gprs.ATCmdC(file_line);
      result=nextLine(CONFIG_file);

      gprs.waitForC(file_line,GSM_MOUT);
   } while (!result);

   hid_led=LO_ALL;
   setLed(hid_led);
}
inline void UploadGPRS()
{
   boolean result;                                 /*Stores return value of gprs methods*/
   switch (gsm_task)
   {
      case 0:
         ++gsm_task;                              /*There is no condition in case 0, only changes subject*/
         /*The upload will be done. Add time stamp*/

         //ASK SIM900's RTC current date*****************
         nextLine(file_download);                  /*Open file to the starting point.*/
         gprs.ATCmdC(file_line);                     /*Sends file_line= "AT+CCLK?"*/

         nextLine(file_download);
         gprs.waitForC(file_line,GSM_MOUT);            /* Waits for file_line= "+CCLK: "*/

         gprs.receiveStream(date,sizeof(date)-1);      /*Reads and copies CCLK to date[]*/
         date[9]='\t';                           /*Changes SIM900 CLK format = "date,hour" to "date   hour"*/

         //SoftSerial.write(date,22);                  /*Prints date[] to confirm copy*/
         //SoftSerial.println();
         gprs.waitFor(OK,GSM_TOUT);                  /*Waits for "OK". It is sent after CCLK sends date stream*/

         //Saves modem changes and restarts modem********
         gprs.setAtCmd(FTPSCONT,OK,GSM_TOUT);         /*Sends from PROGMEM FTPSCONT. FTPSCONT can be included inside the file.
                                             Waits for OK*/
         gprs.ATCmd(RESET);                        /*Sends PROGMEM RESET. RESET can be included inside the file.*/

         gsm_pointer=0;                           /*matchResponse() uses gsm_pointer as a pointer. MatchResponse() call
                                             is similar to waitFor() but there is no main code timeout waiting, moves
                                             timeout comparison to background.*/
         gsm_timer=millis();                        /*starts the timeout.*/
         break;
      case 1:
         if ((millis()-gsm_timer)>GSM_MOUT||gprs.matchResponse(plusCGREG,&gsm_pointer))
         {
            ++gsm_task;
         }
         break;
      case 2:
         nextLine(file_download);
      case 3: //Task 4, send ATcmd from file
         gprs.ATCmdC(file_line);
         if(nextLine(file_download))
         {
            gsm_task=5;
         }
         else
         {
            gsm_task=4;
         }

         gsm_pointer=0;
         gsm_timer=millis();
         break;

      case 4://Task 5, match response from file, occurs when file haven't finished
         if ((millis()-gsm_timer)>GSM_TOUT||gprs.matchResponseC(file_line,&gsm_pointer))
         {
            gsm_task=2;
            //SoftSerial.print("Time: ");
            //SoftSerial.println(gsm_timer,DEC);
         }
         break;
      case 5://Task 5, match response from file, occurs when file have finished
         if ((millis()-gsm_timer)>GSM_TOUT||gprs.matchResponseC(file_line,&gsm_pointer))
         {
            ++gsm_task;
         }
         break;
      case 6:
         gprs.setAtCmd(FTP_PUT_Open,OK,GSM_TOUT);   /*Open a PUT session*/
         ++gsm_task;
         gsm_pointer=0;
         gsm_timer=millis();
         hid_led=BLK_BLU;
         setLed(hid_led);

         break;
      case 7:
         if ((millis()-gsm_timer)>GSM_MOUT||
            gprs.matchResponse(plusFTP_PUT,&gsm_pointer))
         {
            ++gsm_task;
         }
         break;
      case 8:
         result=(millis()-gsm_timer)<GSM_MOUT;      /*Validate if the FTP server responded the FTP session request*/

         if (result)
         {
            result=uploadPacket();                     /*Resolve uploadPacket routine, if the routine was done successfully,
                                                the Dread continue to dump the file uploaded.*/
            if(result)
            {
               DumpFile();                           /*Calls DumpFile Method.Removes current File,
                                                 generates a new File_name*/
               //Uploads new file name to SIM900 Settings**
               buffer[0]='\"';                        /*Add "" to buffer, file_name need "" as the parameter in filename.*/
               buffer[13]='\"';

               for(uint8_t counter=0;counter<12;counter++)   /*Copies file_name on buffer.*/
               {
                  buffer[counter+1]=file_name[counter];
               }

               gprs.setAtParameter(FTP_PUT_Filename,OK,
                              buffer,14,GSM_TOUT);   /*Sends the filename command with file_name copied on buffer*/
            }
            else
            {
               SD.begin(SD_SS);                     /*If the record failed to upload, add the timestamp is added at the
                                                end of the file*/
               file= SD.open(file_name,FILE_WRITE);

               if (file)
               {
                  for (uint8_t i=1;i< 18;++i)            /*Print time stamp. Time stamp length is 17 bytes.
                                                date[] has 1 byte offset, so limits are 1-18*/
                  {
                     file.write(date[i]);
                  }
                  file.println();                     /*Print \r*/
               }
               file.close();
            }
         }

         if(!result)                                 /*If a part of the routine fail, the execution is
                                                stopped and the error is thrown to the HID led,
                                                blinking an violet led.*/
         {
            hid_led=BLK_VLT;
            setLed(hid_led);
         }

         gsm_task++;
         break;
      default:
      gprs.setAtCmd(readCCLK,plusCCLK,GSM_TOUT);
      gprs.receiveStream(date,sizeof(date)-1);      /*Reads and copies CCLK to date[]*/
      date[9]='\t';                           /*Changes SIM900 CLK format = "date,hour" to "date   hour"*/
      gprs.waitFor(OK,GSM_TOUT);
      break;
   }
}
inline void SaveRFID2SD()
{
   boolean result;
   //Check the presence of a S150 Mifare Card
   if (rfid.Request(PICC_REQIDL,buffer)!=MI_ERR)
   {
      //if a card was present, continues the S150 protocol.
      //Reads one (of all the) card(s) that are near the antenna.
      //The S150 read returns block 0, Serial IUD number.
      //In order to avoid saving the same card again, check if last serial number is different from the new serial num.
      //Last serial number is the last card routine that was successfully saved on SD.
      //SoftSerial.println("Req");
      result=rfid.Anticoll(mfr_serial)!=MI_ERR;
      //PrintLong(mfr_serial,0);
      //PrintLong(mfr_lastSer,0);
      //#warning equalStr() was commented
      if (result && !equalStr(mfr_serial,mfr_lastSer))
      {
         //SoftSerial.println("Anti");
         //Uses the serial number to select the S150 to communicate with.
         result=rfid.SelectTag(buffer,mfr_serial)!=MI_ERR;
         if (result)
         {
            //SoftSerial.println("Tag");
            beep();
            //Proceeds with authentication, Nedera uses KeyB, so the method is PICC_AUTHENT1B.}
            result=rfid.Auth(PICC_AUTHENT1B,0x04,Mfr_Key,buffer,mfr_serial)!=MI_ERR;
            if (result)
            {
               //SoftSerial.println("Auth");
               //If success, reads block 0 sector 1. Mifare S150 memory is a vector (not a matrix as how the doc talk about)
               //So each sector has 4 blocks, so each 4 blocks the sector changes. the first 3 block of each sector can be
               //used as memory, the 4th one has the Authentication configuration (keys and modes each block may implement) of
               //the whole sector. Avoid to read or write (EVEN WORSE!!) the 4th block of each sector.
               result=rfid.Read(0x04,buffer)!=MI_ERR;
               if (result)
               {
                  //SoftSerial.println("Read");
                  //After successfully reads the block, the S150 Card is sent back to sleep.
                  rfid.Halt();

                  //Writing File Method
                  //Start SD communication
                  SD.begin(SD_SS);
                  //Check if the file exist by using a string with the name. The method is no case sensitive.
                  //Opens/Creates file by means string with the name. If the file exist the function opens it, else creates it.
                  file = SD.open(file_name, FILE_WRITE);
                  //Check if the file could be opened/created correctly. if Not cancels the saving. HID will ask the user
                  //for a retry.
                  result=file;
                  if (result)
                  {
                     //Prints in file the following format: ID \t RFIDContent(as a decimal number) \t date (dd/mm/yyyy) \t hour (hh:mm) \t Serialnum

                     file.write((uint8_t*)file_download,3);
                     file.write('\t');

                     file.printNumber(PrintLong(buffer,11),DEC);
                     file.write('\t');
                     //Writes date to file.
                     for (uint8_t counter=1;counter<(uint8_t)(sizeof(date)-5);++counter) //date format: "yy/mm/dd \t hh:mm:ss"
                     {
                        file.write(date[counter]);
                        //SoftSerial.write(date[counter]);
                        //SoftSerial.write(',');
                     }
                     //Print function way be able to print date correctly.
                     //file.print(date);
                     file.write('\t');

                     for(uint8_t aux_byte, i=0; i<5;++i)
                     {

                        aux_byte=(mfr_serial[i]>>4)&0x0F;
                        //debug.println(aux_byte,HEX);
                        file.write(aux_byte>9?aux_byte+'A'-10:aux_byte+'0');
                        aux_byte=(mfr_serial[i]&0x0F);
                        file.write(aux_byte>9?aux_byte+'A'-10:aux_byte+'0');
                     }

                     file.println();
                     file.close();
                     //date[9]=',';//change SIM900 CLK format = "date   hour" to "date,hour". Necessary for CCLK matching.
                     //Checks if file was correctly created. Is for debugging purposes only.
                     result=SD.exists(file_name);
                     SoftSerial.write('S');
                     SoftSerial.write('v');
                     SoftSerial.println();
                  }

               }
            }
         }

      }

      setLed(LO_ALL);
      SET_LED=false;
      if (result) {
         //*Commentary function: add /* above this line to comment a full paragraph ****/
         //If the code reaches here, the RFID and SD routines were successful. The HID tells the user he succeed.
         //Correct routine= a long green light with a long beep from buzzer.

         digitalWrite(HID_GLD,HIGH);
         //#warning digitalWrite(HID_BUZZ,HIGH) was commented, it-s annoying!!
         digitalWrite(HID_BUZZ,HIGH); //BEEEEEP, if commented is for debugging purposes, it's annoying!!!
         //Reboots SPI communication, switches to RFID again.
         memcpy(mfr_lastSer,mfr_serial,5);
         delay(800);
         digitalWrite(HID_BUZZ,LOW);
         digitalWrite(HID_GLD,LOW);
         result=false;
         }else {
         //This cases is where the Authentication method failed. HID asks the user for a retry.
         //BEEP BEEP BEEP
         digitalWrite(HID_RLD,HIGH);
         digitalWrite(HID_BUZZ,HIGH);
         delay(200);
         digitalWrite(HID_RLD,LOW);
         digitalWrite(HID_BUZZ,LOW);
         delay(200);
         digitalWrite(HID_RLD,HIGH);
         digitalWrite(HID_BUZZ,HIGH);
         delay(200);
         digitalWrite(HID_RLD,LOW);
         digitalWrite(HID_BUZZ,LOW);
         delay(200);
         digitalWrite(HID_RLD,HIGH);
         digitalWrite(HID_BUZZ,HIGH);
         delay(200);
         digitalWrite(HID_RLD,LOW);
         digitalWrite(HID_BUZZ,LOW);

         SoftSerial.write('U');               /*Print "Un"*/
         SoftSerial.write('n');
         SoftSerial.println();
      }
      setLed(hid_led);//This method set true SET_LED
      delay(500);
      SPI.begin();
      rfid.Init();
   }
   //result chooses HID led result: 500ms large beep/green led pulse for success or 3 short beep/red led pulses if fails.
   //result determine success when all of the methods returned true. If one of those turns false, immediately breaks code
   // and call fail HID routine.


}
void setup()
{
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
   memset(mfr_lastSer,0,5);

   SoftSerial.write('R');            /*Print "RFID"*/
   SoftSerial.write('F');
   SoftSerial.write('I');
   SoftSerial.write('D');
   SoftSerial.println();
}
void loop()
{
   UploadGPRS();
   SaveRFID2SD();
}
