/*
 * RS485.cpp
 *
 * Created: 12/07/2014 11:48:30 a.m.
 *  Author: nedera03
 */ 
#include "RS485.h"
#include "Utilities/DreadSystem.h"

RS485::RS485(uint8_t const write_pin, Print & port)
  : write_pin(write_pin), port(&port)
{}

void RS485::init(void)
{
	pinMode(write_pin, OUTPUT);
	//Set the transceiver to RS mode
	digitalWrite(write_pin, LOW);
}

void RS485::write(uint8_t const * buffer, uint16_t length)
{
	int i;
	char result;
	//Enable Tx RS485
	digitalWrite(write_pin, HIGH);
	//delayMicroseconds(100);//Waits until transmit holding register empties.
	//Waits until transmit holding register empties.
	for (i=0;i<length;i++)
	{
		port->write(buffer[i]);
	}
	while (!(UCSR0A & (1<<TXC0))){}
	//delayMicroseconds(400);//Waits until transmit holding register empties.
	//Enable Rx RS485
	digitalWrite(write_pin,LOW);
}

int RS485::read(void) 
{
  digitalWrite(write_pin, LOW);
  return port->read();
}

void RS485::write(uint8_t c) {
  digitalWrite(write_pin, HIGH);
  port->write(c);
}

int RS485::available(void)
{
  return port->available();
}
