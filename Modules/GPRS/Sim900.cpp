/*
 * Sim900.cpp
 *
 * Created: 12/02/2014 04:43:04 p.m.
 *  Author: nedera03
 */ 
#include "Sim900.h"

const uint8_t AT[5] PROGMEM ="AT+\r";
const uint8_t CONNECT[12] PROGMEM = "Connecting\r";

Sim900::Sim900(HardwareSerial&hard_port,SoftwareSerial&soft_port)
{
	_soft_port=&soft_port;
	_hard_port=&hard_port;	
}

void Sim900::begin(const uint16_t baud_rate,const uint8_t rst_pin,const uint8_t nopowerdown_pin)
{
	uint8_t ptr;
	_hard_port->begin(baud_rate);
	_soft_port->begin(baud_rate);
	for (ptr=0; pgm_read_byte(CONNECT+ptr)!='\r';++ptr)
	{
		_soft_port->write(pgm_read_byte(CONNECT+ptr));
	}
	_soft_port->println();
	pinMode(rst_pin,OUTPUT);
	pinMode(nopowerdown_pin,OUTPUT);
	digitalWrite(rst_pin,HIGH);
	digitalWrite(nopowerdown_pin,HIGH);
	delay(50);
	digitalWrite(rst_pin,LOW);//Reset SIM900 quicker
}

void Sim900::ATCmd(const uint8_t*atcmd)
{
	_hard_port->flush();
	printP(AT);
	printP(atcmd);
	
	_hard_port->println();
}

void Sim900::ATCmdC(const uint8_t*atcmd)
{
	_hard_port->flush();
	
	while (*atcmd!='\r')
	{
		_hard_port->write(*atcmd);
		++atcmd;
	}
	
	_hard_port->println();
}

boolean Sim900::matchResponse(const uint8_t*expected_response,uint8_t*ptr)
{
	boolean result=false;
	uint8_t receive_byte;
	while (_hard_port->available()>0 && !result)
	{
			receive_byte=_hard_port->read();
			//Compare, if the char is equal, the pointer advance, otherwise, it resets to zero.
			if(receive_byte==pgm_read_byte(expected_response+(*ptr))){
				++(*ptr);
				}else{
				(*ptr)=0;
			}
			//Determine if the ptr
			result= pgm_read_byte(expected_response+(*ptr))=='\r';
			_soft_port->write(receive_byte);
			//wdt_reset();
	}
	return result;
}

boolean Sim900::matchResponseC(const uint8_t*expected_response,uint8_t*ptr)
{
	boolean result=false;
	//Result may return:
	//0 for no match found
	//1 for no response
	//2 for success
	uint8_t receive_byte;
	
	while (_hard_port->available()>0 && !result)
	{
		receive_byte=_hard_port->read();
		
		if(receive_byte==expected_response[(*ptr)]){
			++(*ptr);
			}else{
			(*ptr)=0;
		}
		//Determine if the ptr
		result= expected_response[(*ptr)]=='\r';
		_soft_port->write(receive_byte);
		//wdt_reset();
	}
	return result;
}

boolean Sim900::setAtCmd(const uint8_t*atcmd,const uint8_t*expected_response,
						 const uint32_t timeout)
{
	//Clean buffer
	//_soft_port->println("Clean buff");
	_hard_port->flush();
	printP(AT);
	printP(atcmd);
	
	_hard_port->println();
	//_soft_port->println();
	//_soft_port->println("ANSWER: ");
	return waitFor(expected_response,timeout);
}

boolean Sim900::setAtParameter(const uint8_t*atcmd,const uint8_t*expected_reponse,
							   const uint8_t*set_parameter,const uint8_t parameter_size,
							   const uint32_t timeout)
{
	_hard_port->flush();
	printP(AT);
	printP(atcmd);
	
	send(set_parameter,parameter_size);
	_hard_port->println();
	//_soft_port->println();
	return waitFor(expected_reponse,timeout);
}

boolean Sim900::setAtParameter(const uint8_t*atcmd,const uint8_t*expected_reponse,
							   const uint16_t parameter_num,const uint32_t timeout)
{
	_hard_port->flush();
	printP(AT);
	printP(atcmd);
	//send(set_parameter,parameter_size);
	_hard_port->printNumber((uint32_t)parameter_num,DEC);
	_hard_port->println();
	_soft_port->printNumber((uint32_t)parameter_num,DEC);
	_soft_port->println();
	return waitFor(expected_reponse,timeout);
}

void Sim900::hard2Soft(void)
{
	if (_hard_port->available()>0)
	{
		_soft_port->write(_hard_port->read());
	}
}

void Sim900::soft2Hard(void)
{
	if (_soft_port->available()>0)
	{
		_hard_port->write(_soft_port->read());
	}
}

boolean Sim900::waitFor(const uint8_t*expected_response,const uint32_t timeout)
{
	uint32_t time=millis();
	boolean success=false;
	uint8_t ptr=0;
	
	while((millis()-time)<timeout && !success)
	{
		success=matchResponse(expected_response,&ptr);
	}
	_soft_port->println();
	
	return success;
}

boolean Sim900::waitForC(const uint8_t*expected_response,const uint32_t timeout)
{
	uint32_t time=millis();
	boolean success=false;
	uint8_t ptr=0;
	while((millis()-time)<timeout && !success)
	{
		success=matchResponseC(expected_response,&ptr);
	}
	_soft_port->println();
	return success;
}

boolean Sim900::receiveStream(uint8_t*stream,uint16_t length)
{
	while (length)
	{
		if (_hard_port->available()>0)
		{
			*stream=_hard_port->read();
			_soft_port->write(*stream);
			++stream;
			--length;
		}
	}
	return length==0;
}

void Sim900::send(const uint8_t*buffer,uint16_t send_len)
{
	while (send_len--)
	{
		_hard_port->write(*buffer);
		++buffer;
	}
		
}

void Sim900::printP(const uint8_t*atcmd)
{
	for (uint8_t ptr=0; pgm_read_byte(atcmd+ptr)!='\r';++ptr)
	{
		_hard_port->write(pgm_read_byte(atcmd+ptr));
		//_soft_port->write(pgm_read_byte(atcmd+ptr));
	}
}