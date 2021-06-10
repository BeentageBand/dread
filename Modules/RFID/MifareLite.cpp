/*
 * MifareLite.cpp
 *
 *  Created on: 21-feb-2014
 *      Author: Puch
 */

#include "MifareLite.h"

#include "SPI.h"

#define MAX_LEN 16

//------------------MFRC522 register ---------------
// Page 0:Command and Status
#define Reserved00 0x00
#define CommandReg 0x01
#define CommIEnReg 0x02
#define DivlEnReg 0x03
#define CommIrqReg 0x04
#define DivIrqReg 0x05
#define ErrorReg 0x06
#define Status1Reg 0x07
#define Status2Reg 0x08
#define FIFODataReg 0x09
#define FIFOLevelReg 0x0A
#define WaterLevelReg 0x0B
#define ControlReg 0x0C
#define BitFramingReg 0x0D
#define CollReg 0x0E
#define Reserved01 0x0F

// Page 1:Command
#define Reserved10 0x10
#define ModeReg 0x11
#define TxModeReg 0x12
#define RxModeReg 0x13
#define TxControlReg 0x14
#define TxAutoReg 0x15
#define TxSelReg 0x16
#define RxSelReg 0x17
#define RxThresholdReg 0x18
#define DemodReg 0x19
#define Reserved11 0x1A
#define Reserved12 0x1B
#define MifareReg 0x1C
#define Reserved13 0x1D
#define Reserved14 0x1E
#define SerialSpeedReg 0x1F

// Page 2:CFG
#define Reserved20 0x20
#define CRCResultRegM 0x21
#define CRCResultRegL 0x22
#define Reserved21 0x23
#define ModWidthReg 0x24
#define Reserved22 0x25
#define RFCfgReg 0x26
#define GsNReg 0x27
#define CWGsPReg 0x28
#define ModGsPReg 0x29
#define TModeReg 0x2A
#define TPrescalerReg 0x2B
#define TReloadRegH 0x2C
#define TReloadRegL 0x2D
#define TCounterValueRegH 0x2E
#define TCounterValueRegL 0x2F

// Page 3:TestRegister
#define Reserved30 0x30
#define TestSel1Reg 0x31
#define TestSel2Reg 0x32
#define TestPinEnReg 0x33
#define TestPinValueReg 0x34
#define TestBusReg 0x35
#define AutoTestReg 0x36
#define VersionReg 0x37
#define AnalogTestReg 0x38
#define TestDAC1Reg 0x39
#define TestDAC2Reg 0x3A
#define TestADCReg 0x3B
#define Reserved31 0x3C
#define Reserved32 0x3D
#define Reserved33 0x3E
#define Reserved34 0x3F

//-----------------------------------------------
/*
//4 bytes Serial number of card, the 5 bytes is verify bytes
uint8_t writeData[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100 };
//initialize to 100 USD

//buffer A password, 16 buffer, the password of every buffer is 6 byte
uint8_t sectorKeyA[16][16] = {
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
//{0x19, 0x84, 0x07, 0x15, 0x76, 0x14},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
};

uint8_t sectorNewKeyA[16][16] = {
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
{0xFF, 0xFF, 0xFF, 0xFF,
 0xFF, 0xFF, 0xff, 0x07,
 0x80, 0x69, 0x19, 0x84,
 0x07, 0x15, 0x76, 0x14 },
//you can set another key , such as  " 0x19, 0x84, 0x07, 0x15, 0x76, 0x14 "
//{0x19, 0x84, 0x07, 0x15, 0x76, 0x14, 0xff,0x07,0x80,0x69,
0x19,0x84,0x07,0x15,0x76,0x14},

// but when loop, please set the  sectorKeyA, the same key, so that RFID module
can read the card {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xff, 0x07, 0x80, 0x69,
0x19, 0x33, 0x07, 0x15, 0x34, 0x14},
};
*/

MifareLite::MifareLite(uint8_t PinSS, uint8_t PinRST) {
  _pinSlaveSelect = PinSS; // SDA
  _pinReset = PinRST;      // RST
}
// MifareLite::~MifareLite() {
//
//}

/*
 * Function?ResetMFRC522
 * Description? reset RC522
 * Input parameter?null
 * Return?null
 */
void MifareLite::Reset(void) { Write_MFRC522(CommandReg, PCD_RESETPHASE); }

/*
 * Function?InitMFRC522
 * Description?initilize RC522
 * Input parameter?null
 * Return?null
 */
void MifareLite::Init(void) {
  // Setup the Pins
  pinMode(_pinSlaveSelect, OUTPUT);   // Set digital pin 10 as OUTPUT to connect
                                      // it to the RFID /ENABLE pin
  digitalWrite(_pinSlaveSelect, LOW); // Activate the RFID reader
  pinMode(_pinReset, OUTPUT); // Set digital pin 10 , Not Reset and Power-down
  digitalWrite(_pinReset, LOW);
  // activate
  digitalWrite(_pinReset, HIGH);

  Reset();

  // Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  Write_MFRC522(TModeReg, 0x8D);      // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
  Write_MFRC522(TPrescalerReg, 0x3E); // TModeReg[3..0] + TPrescalerReg
  Write_MFRC522(TReloadRegL, 30);
  Write_MFRC522(TReloadRegH, 0);
  Write_MFRC522(TxAutoReg, 0x40); // 100%ASK
  Write_MFRC522(ModeReg, 0x3D);   // CRC initilizate value 0x6363	???

  // ClearBitMask(Status2Reg, 0x08);		//MFCrypto1On=0
  // Write_MFRC522(RxSelReg, 0x86);		//RxWait = RxSelReg[5..0]
  // Write_MFRC522(RFCfgReg, 0x7F);   		//RxGain = 48dB
  AntennaOn(); // turn on antenna
}

/*
 * Function?MFRC522_Request
 * Description?Searching card, read card type
 * Input parameter?reqMode--search methods?
 *			 TagType--return card types
 *			 	0x4400 = Mifare_UltraLight
 *				0x0400 = Mifare_One(S50)
 *				0x0200 = Mifare_One(S70)
 *				0x0800 = Mifare_Pro(X)
 *				0x4403 = Mifare_DESFire
 * return?return MI_OK if successed
 */
uint8_t MifareLite::Request(uint8_t reqMode, uint8_t *TagType) {
  uint8_t status;
  uint8_t backBits; // the data bits that received
  Write_MFRC522(BitFramingReg,
                0x07); // TxLastBists = BitFramingReg[2..0]	???
  TagType[0] = reqMode;
  status = ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
  if ((status != MI_OK) || (backBits != 0x10)) {
    status = MI_ERR;
  }

  return status;
}

/*
 * Function?MFRC522_ToCard
 * Description?communicate between RC522 and ISO14443
 * Input parameter?command--MF522 command bits
 *			 sendData--send data to card via rc522
 *			 sendLen--send data length
 *			 backData--the return data from card
 *			 backLen--the length of return data
 * return?return MI_OK if successed
 */
uint8_t MifareLite::ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen,
                           uint8_t *backData, uint8_t *backLen) {
  uint8_t status = MI_ERR;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits;
  uint8_t n;
  uint8_t i;
  switch (command) {
  case PCD_AUTHENT: // verify card password
  {
    irqEn = 0x12;
    waitIRq = 0x10;
    break;
  }
  case PCD_TRANSCEIVE: // send data in the FIFO
  {
    irqEn = 0x77;
    waitIRq = 0x30;
    break;
  }
  default:
    break;
  }

  Write_MFRC522(CommIEnReg, irqEn | 0x80); // Allow interruption
  ClearBitMask(CommIrqReg, 0x80);          // Clear all the interrupt bits
  SetBitMask(FIFOLevelReg, 0x80);          // FlushBuffer=1, FIFO initilizate
  Write_MFRC522(CommandReg, PCD_IDLE);     // NO action;cancel current command
                                           // ???

  // write data into FIFO
  for (i = 0; i < sendLen; i++) {
    Write_MFRC522(FIFODataReg, sendData[i]);
  }

  // procceed it
  Write_MFRC522(CommandReg, command);
  if (command == PCD_TRANSCEIVE) {
    SetBitMask(BitFramingReg, 0x80); // StartSend=1,transmission of data starts
  }

  // waite receive data is finished
  i = 2000; // i should adjust according the clock, the maxium the waiting time
            // should be 25 ms???
  do {
    // CommIrqReg[7..0]
    // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = Read_MFRC522(CommIrqReg);
    i--;
  }

  while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

  ClearBitMask(BitFramingReg, 0x80); // StartSend=0
  if (i != 0) {
    if (!(Read_MFRC522(ErrorReg) &
          0x1B)) { // BufferOvfl Collerr CRCErr ProtecolErr
      status = MI_OK;
      if (n & irqEn & 0x01) {
        status = MI_NOTAGERR; //??
      }

      if (command == PCD_TRANSCEIVE) {
        n = Read_MFRC522(FIFOLevelReg);
        lastBits = Read_MFRC522(ControlReg) & 0x07;
        if (lastBits) {
          *backLen = (n - 1) * 8 + lastBits;
        } else {
          *backLen = n * 8;
        }

        if (n == 0) {
          n = 1;
        }

        if (n > MAX_LEN) {
          n = MAX_LEN;
        }

        // read the data from FIFO
        for (i = 0; i < n; i++) {
          backData[i] = Read_MFRC522(FIFODataReg);
        }
      }
    } else {
      status = MI_ERR;
    }
  }

  // SetBitMask(ControlReg,0x80);           //timer stops
  // Write_MFRC522(CommandReg, PCD_IDLE);
  return status;
}

/*
 * Function?MFRC522_Anticoll
 * Description?Prevent conflict, read the card serial number
 * Input parameter?serNum--return the 4 bytes card serial number, the 5th byte
 * is recheck byte return?return MI_OK if successed
 */
uint8_t MifareLite::Anticoll(uint8_t *serNum) {
  uint8_t status;
  int8_t i;
  uint8_t serNumCheck = 0;
  uint8_t unLen;
  // ClearBitMask(Status2Reg, 0x08);		//TempSensclear
  // ClearBitMask(CollReg,0x80);			//ValuesAfterColl
  Write_MFRC522(BitFramingReg, 0x00); // TxLastBists = BitFramingReg[2..0]
  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  status = ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
  if (status == MI_OK) {
    // Verify card serial number
    for (i = 0; i < 4; i++) {
      serNumCheck ^= serNum[i];
    }
    if (serNumCheck != serNum[i]) {
      status = MI_ERR;
    }
  }
  // SetBitMask(CollReg, 0x80);		//ValuesAfterColl=1
  return status;
}

/*
 * Function?MFRC522_SelectTag
 * Description?Select card, read card storage volume
 * Input parameter?serNum--Send card serial number
 * return?return the card storage volume
 */
uint8_t MifareLite::SelectTag(uint8_t *buffer, uint8_t *serNum) {
  uint8_t i;
  uint8_t status;
  uint8_t size;
  uint8_t recvBits;
  // ClearBitMask(Status2Reg, 0x08);			//MFCrypto1On=0
  buffer[0] = PICC_SElECTTAG;
  buffer[1] = 0x70;
  for (i = 0; i < 5; i++) {
    buffer[i + 2] = *(serNum + i);
  }

  CalulateCRC(buffer, 7, &buffer[7]); //??
  status = ToCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
  if ((status == MI_OK) && (recvBits == 0x18)) {
    size = buffer[0];
  } else {
    size = 0;
  }
  return size;
}

/*
 * Function?MFRC522_Auth
 * Description?verify card password
 * Input parameters?authMode--password verify mode
 0x60 = verify A password key
 0x61 = verify B password key
 BlockAddr--Block address
 Sectorkey--Block password
 serNum--Card serial number ?4 bytes
 * return?return MI_OK if successes
 */
uint8_t MifareLite::Auth(uint8_t authMode, uint8_t BlockAddr,
                         uint8_t *Sectorkey, uint8_t *buff, uint8_t *serNum) {
  uint8_t status;
  uint8_t recvBits;
  uint8_t i;
  // Verify command + block address + buffer password + card SN
  buff[0] = authMode;
  buff[1] = BlockAddr;
  for (i = 0; i < 6; i++) {
    buff[i + 2] = *(Sectorkey + i);
  }

  for (i = 0; i < 4; i++) {
    buff[i + 8] = *(serNum + i);
  }

  status = ToCard(PCD_AUTHENT, buff, 12, buff, &recvBits);
  if ((status != MI_OK) || (!(Read_MFRC522(Status2Reg) & 0x08))) {
    status = MI_ERR;
  }
  return status;
}

/*
 * Function?MFRC522_Read
 * Description?Read data
 * Input parameters?blockAddr--block address;recvData--the block data which are
 * read return?return MI_OK if successes
 */
uint8_t MifareLite::Read(uint8_t blockAddr, uint8_t *recvData) {
  uint8_t status;
  uint8_t unLen;
  recvData[0] = PICC_READ;
  recvData[1] = blockAddr;
  CalulateCRC(recvData, 2, &recvData[2]);
  status = ToCard(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);
  if ((status != MI_OK) || (unLen != 0x90)) {
    status = MI_ERR;
  }
  return status;
}

/*FOR DREAD IS NOT NECESSARY TO WRITE IN MIFARE CARDS
 * Function?MFRC522_Write
 * Description?write block data
 * Input parameters?blockAddr--block address;writeData--Write 16 bytes data into
 * block return?return MI_OK if successes
 */
uint8_t MifareLite::Write(uint8_t blockAddr, uint8_t *writeData) {
  uint8_t status;
  uint8_t recvBits;
  int8_t i;
  uint8_t buff[18];
  buff[0] = PICC_WRITE;
  buff[1] = blockAddr;
  CalulateCRC(buff, 2, &buff[2]);
  status = ToCard(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);
  if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
    status = MI_ERR;
  }
  if (status == MI_OK) {
    for (i = 0; i < 16; i++) { // Write 16 bytes data into FIFO
      buff[i] = *(writeData + i);
    }

    CalulateCRC(buff, 16, &buff[16]);
    status = ToCard(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);
    if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
      status = MI_ERR;
    }
  }

  return status;
}

/*
 * Function?MFRC522_Halt
 * Description?Command the cards into sleep mode
 * Input parameters?null
 * return?null
 */
void MifareLite::Halt(void) {
  uint8_t unLen;
  uint8_t buff[4];
  buff[0] = PICC_HALT;
  buff[1] = 0;
  CalulateCRC(buff, 2, &buff[2]);
  ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}

/*
 * Function?Write_MFRC5200
 * Description?write a byte data into one register of MR RC522
 * Input parameter?addr--register address?val--the value that need to write in
 * Return?Null
 */
void MifareLite::Write_MFRC522(uint8_t addr, uint8_t val) {
  digitalWrite(_pinSlaveSelect, LOW);
  // address format?0XXXXXX0
  SPI.transfer((addr << 1) & 0x7E);
  SPI.transfer(val);
  digitalWrite(_pinSlaveSelect, HIGH);
}

/*
 * Function?Read_MFRC522
 * Description?read a byte data into one register of MR RC522
 * Input parameter?addr--register address
 * Return?return the read value
 */
uint8_t MifareLite::Read_MFRC522(uint8_t addr) {
  uint8_t val;
  digitalWrite(_pinSlaveSelect, LOW);
  // address format?1XXXXXX0
  SPI.transfer(((addr << 1) & 0x7E) | 0x80);
  val = SPI.transfer(0x00);
  digitalWrite(_pinSlaveSelect, HIGH);
  return val;
}

/*
 * Function?CalulateCRC
 * Description?Use MF522 to caculate CRC
 * Input parameter?pIndata--the CRC data need to be read?len--data
 * length?pOutData-- the caculated result of CRC return?Null
 */
void MifareLite::CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData) {
  uint8_t i, n;
  ClearBitMask(DivIrqReg, 0x04);  // CRCIrq = 0
  SetBitMask(FIFOLevelReg, 0x80); // Clear FIFO pointer
  // Write_MFRC522(CommandReg, PCD_IDLE);
  // Write data into FIFO
  for (i = 0; i < len; i++) {
    Write_MFRC522(FIFODataReg, *(pIndata + i));
  }
  Write_MFRC522(CommandReg, PCD_CALCCRC);

  // waits CRC calculation to finish
  i = 0xFF;
  do {
    n = Read_MFRC522(DivIrqReg);
    i--;
  }

  while ((i != 0) && !(n & 0x04)); // CRCIrq = 1

  // read CRC calculation result
  pOutData[0] = Read_MFRC522(CRCResultRegL);
  pOutData[1] = Read_MFRC522(CRCResultRegM);
}

/*
 * Function?SetBitMask
 * Description?set RC522 register bit
 * Input parameter?reg--register address;mask--value
 * Return?null
 */
void MifareLite::SetBitMask(uint8_t reg, uint8_t mask) {
  uint8_t tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, tmp | mask); // set bit mask
}

/*
 * Function?ClearBitMask
 * Description?clear RC522 register bit
 * Input parameter?reg--register address;mask--value
 * Return?null
 */
void MifareLite::ClearBitMask(uint8_t reg, uint8_t mask) {
  uint8_t tmp;
  tmp = Read_MFRC522(reg);
  Write_MFRC522(reg, tmp & (~mask)); // clear bit mask
}

/*
 * Function?AntennaOn
 * Description?Turn on antenna, every time turn on or shut down antenna need at
 * least 1ms delay Input parameter?null Return?null
 */
void MifareLite::AntennaOn(void) {
  uint8_t temp;
  temp = Read_MFRC522(TxControlReg);
  if (!(temp & 0x03)) {
    SetBitMask(TxControlReg, 0x03);
  }
}

/*
 * Function?AntennaOff
 * Description?Turn off antenna, every time turn on or shut down antenna need at
 * least 1ms delay Input parameter?null Return?null
 */
void MifareLite::AntennaOff(void) { ClearBitMask(TxControlReg, 0x03); }
