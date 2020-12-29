/*
 * Sim900.h
 *
 * Created: 12/02/2014 04:37:19 p.m.
 *  Author: nedera03
 */ 



#ifndef SIM900_H_
#define SIM900_H_
//******************************LIBRARIES****************************//
//#include <Arduino.h>
#include "DreadSystem.h"
#include "DreadConfig.h"
#include "SoftwareSerial.h"
#include "HardwareSerial.h"
//*****************************DEFINITIONS***************************//
#define AT_READ  '?'
#define AT_WRITE '='
#define AT_EXEC	 0x00
//*****************************PROGMEM*******************************//

class Sim900{
	public:
	Sim900(HardwareSerial & hard_port,SoftwareSerial & soft_port);
	void begin(const uint16_t baud_rate,const uint8_t rst_pin,const uint8_t nopowerdown_pin);
	void ATCmd(const uint8_t*atcmd);
	void ATCmdC(const uint8_t*atcmd);
	boolean setAtCmd(const uint8_t*atcmd,const uint8_t*expected_response,const uint32_t timeout);
	boolean matchResponse(const uint8_t*expected_response,uint8_t & ptr);
	boolean matchResponseC(const uint8_t*expected_response,uint8_t & ptr);
	boolean waitFor(const uint8_t*expected_response,uint32_t timeout);
	boolean waitForC(const uint8_t*expected_response,uint32_t timeout);
	void send(const uint8_t*buffer,uint16_t send_len);
	boolean setAtParameter(const uint8_t*atcmd,const uint8_t*expected_reponse,const uint8_t*set_parameter,const uint8_t parameter_size,const uint32_t timeout);
	boolean setAtParameter(const uint8_t*atcmd,const uint8_t*expected_reponse,const uint16_t parameter_num,const uint32_t timeout);
	boolean receiveStream(uint8_t*stream,uint16_t length);
	void hard2Soft(void);
	void soft2Hard(void);
	inline Print & getPrint(void) {return *_hard_port; }
	private:
	void printP(const uint8_t*atcmd);
	SoftwareSerial *_soft_port;
	HardwareSerial *_hard_port;
	};
#endif /* SIM900_H_ */
