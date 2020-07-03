/*
 * SC16IS740.cpp
 *
 * Created: 05/03/2014 03:41:38 p.m.
 *  Author: nedera03
 */ 
#include "SC16IS740.h"
#include "Drivers/SPI/SPI.h"

SC16IS740::SC16IS740(uint8_t select_pin/*,HardwareSerial&port*/,SoftwareSerial&port)
{
	_ss_pin=select_pin;
	_port=&port;
}

void SC16IS740::init(uint8_t xtal_freq)
{
	//_port->begin(debug_baudrate);
	//_port->println("InitSC16");
	pinMode(_ss_pin,OUTPUT);
	write_SC16IS(LCR, 0x80); // 0x80 to program baud rate
	
	write_SC16IS(DLL, xtal_freq); // 0x06=19.2K with X1=1.8432 MHz
	
	write_SC16IS(DLH, 0x00); // divisor = 0x0006 for 19200 bps
	write_SC16IS(LCR, 0xBF); // access EFR register
	write_SC16IS(EFR, 0X10); // enable enhanced registers
	write_SC16IS(LCR, 0x03); // 8 data bit, 1 stop bit, no parity
	write_SC16IS(FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
	delay(500);
	write_SC16IS(FCR, 0x01); // enable FIFO mode
	write_SC16IS(MCR,0x02); //Enable ~RTS, So RTS SC enables R485 to receiveData
}
void SC16IS740::flush()
{
	write_SC16IS(FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
	write_SC16IS(FCR, 0x01); // enable FIFO mode
}
uint8_t SC16IS740::read()
{
	if ((read_SC16IS(LSR)&0x01)==1)
	{
		return read_SC16IS(RHR);
	}else
	{
		return -1;
	}
}

uint8_t SC16IS740::matchResponse(uint8_t*response,const uint8_t first_char,const uint8_t last_char,const uint8_t response_size,uint8_t* res_ptr)
{
	return -1;
}
	
uint8_t SC16IS740::processResponse(uint8_t*response,const uint8_t first_char,const uint8_t last_char,const uint8_t response_size,const uint16_t timeout)
{
	uint8_t pointer=0;
	uint16_t init_time;
	uint16_t time;
	memset(response,0,response_size); //Set to zeros the response (clean buffer)
	//Wait until bytes come from the
	init_time=(uint16_t) millis();
	do
	{
		if(available()>0)
		{
			*response=read();//Read until initial character match
			//_port->write(response[pointer]);
		}
		time=(uint16_t)(millis()-init_time);
	} while (*response!=first_char && time<timeout);
	//_port->println("first matched");
	do 
	{
		if(available()>0)
		{
			response[pointer]=read();//Read until initial character match
			//_port->write(response[pointer]);
			++pointer;
		}
		time=millis()-init_time;
	} while (response[pointer-1]!=last_char && pointer<response_size && time<timeout);
	//_port->print("Time:");
	//_port->println(time,DEC);
	//Return if the message matches length,initial and final characters, in other words, matches format.
	if (pointer==response_size && response[response_size-1]==last_char)
	{
		return SC_RESPOND_RECEIVED; //Response matches format
	}
	else if(pointer>=response_size)
	{
		return SC_RESPOND_OUTOFBOUND;
	}
	else if (time>=timeout)
	{
		return SC_RESPOND_TIMEOUT;
	}
	else
	{
		return SC_RESPOND_NOFORMAT; //Receives a
	}
	/*_port->write("Left on port:",11);
	_port->printNumber(available(),DEC);
	_port->println();*/
}
void SC16IS740::send(const uint8_t*buffer,uint8_t length)
{
	write_SC16IS(MCR,0x00);//Enable Tx RS485
	digitalWrite(_ss_pin,LOW);
	SPI.transfer(THR);
	while (length--)
	{
		SPI.transfer(*buffer);
		++buffer;
	}
	digitalWrite(_ss_pin,HIGH);
	do
	{	
		length=read_SC16IS(LSR)&0x20; /*recycles length to obtain result*/
	} while (length!=0x20);
	delayMicroseconds(400);//Waits until transmit holding register empties.
	write_SC16IS(MCR,0x02);//Enable Rx RS485
}
void SC16IS740::sendP(const uint8_t*buffer)
{
	uint8_t i=0;
	write_SC16IS(MCR,0x00);//Enable Tx RS485
	digitalWrite(_ss_pin,LOW);
	SPI.transfer(THR);
	do
	{
		SPI.transfer(pgm_read_byte(buffer+i));
		++i;
	}
	while(pgm_read_byte(buffer+i)!='\r');
	digitalWrite(_ss_pin,HIGH);
	do
	{
		i=read_SC16IS(LSR)&0x20;	/*recycles i to obtain result*/
	} while (i!=0x20);
	delayMicroseconds(400);//Waits until transmit holding register empties.
	write_SC16IS(MCR,0x02);//Enable Rx RS485
}
uint8_t SC16IS740::available()
{
	return read_SC16IS(RXLVL);
}
void SC16IS740::write_SC16IS(uint8_t add,uint8_t val)
{
	digitalWrite(_ss_pin,LOW);
	SPI.transfer(add);
	SPI.transfer(val);
	digitalWrite(_ss_pin,HIGH);
}
uint8_t SC16IS740::read_SC16IS(uint8_t add)
{
	uint8_t data;
	digitalWrite(_ss_pin,LOW);
	SPI.transfer(add|0x80);
	data=SPI.transfer(0);
	digitalWrite(_ss_pin,HIGH);
	return data;
}
