/*
 * MifareLite.h
 *
 * Created: 21/02/2014 12:03:45 p.m.
 *  Author: Puch
 */ 


#ifndef MIFARELITE_H_
#define MIFARELITE_H_

#include "DreadSystem.h"

//MF522 command bits
#define PCD_IDLE              0x00               //NO action; cancel current commands
#define PCD_AUTHENT           0x0E               //verify password key
#define PCD_RECEIVE           0x08               //receive data
#define PCD_TRANSMIT          0x04               //send data
#define PCD_TRANSCEIVE        0x0C               //send and receive data
#define PCD_RESETPHASE        0x0F               //reset
#define PCD_CALCCRC           0x03               //CRC check and caculation

//Mifare_One card command bits
#define PICC_REQIDL           0x26               //Search the cards that not into sleep mode in the antenna area
#define PICC_REQALL           0x52               //Search all the cards in the antenna area
#define PICC_ANTICOLL         0x93               //prevent conflict
#define PICC_SElECTTAG        0x93               //select card
#define PICC_AUTHENT1A        0x60               //verify A password key
#define PICC_AUTHENT1B        0x61               //verify B password key
#define PICC_READ             0x30               //read
#define PICC_WRITE            0xA0               //write
#define PICC_DECREMENT        0xC0               //deduct value
#define PICC_INCREMENT        0xC1               //charge up value
#define PICC_RESTORE          0xC2               //Restore data into buffer
#define PICC_TRANSFER         0xB0               //Save data into buffer
#define PICC_HALT             0x50               //sleep mode

//THe mistake code that return when communicate with MF522
#define MI_OK                 0
#define MI_NOTAGERR           1
#define MI_ERR                2



class MifareLite {
	public:
	MifareLite(uint8_t PinSS,uint8_t PinRST);
	//	virtual ~MifareLite();
	void Reset(void);
	void Init(void);
	uint8_t Request(uint8_t reqMode, uint8_t *TagType);
	uint8_t Anticoll(uint8_t *serNum);
	uint8_t SelectTag(uint8_t *buffer, uint8_t *serNum);
	//uint8_t SelectTag(uint8_t *serNum);
	uint8_t Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *buff, uint8_t *serNum);
	//uint8_t Auth(uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum);
	uint8_t Read(uint8_t blockAddr, uint8_t *recvData);
	uint8_t Write(uint8_t blockAddr, uint8_t *writeData);
	//String ParseStr(uint8_t *inData,uint8_t dataLen);
	void Halt(void);

	private:
	void Write_MFRC522(uint8_t addr, uint8_t val);
	uint8_t Read_MFRC522(uint8_t addr);
	uint8_t ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen,uint8_t *backData, uint8_t *backLen);
	void CalulateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);
	void SetBitMask(uint8_t reg, uint8_t mask);
	void ClearBitMask(uint8_t reg, uint8_t mask);
	void AntennaOn(void);
	void AntennaOff(void);

	int _pinReset;
	int _pinSlaveSelect;
};


#endif /* MIFARELITE_H_ */
