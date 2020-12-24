/*
 * SC16IS740.cpp
 *
 * Created: 05/03/2014 03:41:38 p.m.
 *  Author: nedera03
 */ 
#include "SC16IS740.h"
#include "SPI.h"
#include "DreadSystem.h"

SC16IS740::SC16IS740(uint8_t const ss_pin)
  : ss_pin(ss_pin) 
{}

void SC16IS740::init(uint8_t const xtal_freq)
{
	//logger->begin(debug_baudrate);
	//logger->println("InitSC16");
	pinMode(ss_pin,OUTPUT);
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

int SC16IS740::read()
{
	if ((read_SC16IS(LSR)&0x01)==1)
	{
		return read_SC16IS(RHR);
	}else
	{
		return -1;
	}
}

void SC16IS740::write(uint8_t const * buffer, uint16_t length)
{
	write_SC16IS(MCR,0x00);//Enable Tx RS485
	digitalWrite(ss_pin,LOW);
	SPI.transfer(THR);
	while (length--)
	{
		SPI.transfer(*buffer);
		++buffer;
	}
	digitalWrite(ss_pin,HIGH);
	do
	{	
		length=read_SC16IS(LSR)&0x20; /*recycles length to obtain result*/
	} while (length!=0x20);
	delayMicroseconds(400);//Waits until transmit holding register empties.
	write_SC16IS(MCR,0x02);//Enable Rx RS485
}

void SC16IS740::write(uint8_t c) {
	write_SC16IS(MCR,0x00);//Enable Tx RS485
	digitalWrite(ss_pin,LOW);
	SPI.transfer(THR);
  SPI.transfer(c);
	digitalWrite(ss_pin,HIGH);
  while(0x20 == (read_SC16IS(LSR)&0x20)) {}
	delayMicroseconds(400);//Waits until transmit holding register empties.
	write_SC16IS(MCR,0x02);//Enable Rx RS485
}

int SC16IS740::available()
{
	return read_SC16IS(RXLVL);
}

void SC16IS740::write_SC16IS(uint8_t add,uint8_t val)
{
	digitalWrite(ss_pin,LOW);
	SPI.transfer(add);
	SPI.transfer(val);
	digitalWrite(ss_pin,HIGH);
}

uint8_t SC16IS740::read_SC16IS(uint8_t add)
{
	uint8_t data;
	digitalWrite(ss_pin,LOW);
	SPI.transfer(add|0x80);
	data=SPI.transfer(0);
	digitalWrite(ss_pin,HIGH);
	return data;
}
