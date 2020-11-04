/*
 * RS485.cpp
 *
 * Created: 12/07/2014 11:48:30 a.m.
 *  Author: nedera03
 */ 
#include "RS485.h"
#include "Utilities/DreadSystem.h"

RS485::RS485(uint8_t const ss_pin, Print & port, Print & logger)
  : ss_pin(ss_pin), port(&port), logger(&logger)
{}

void RS485::init()
{
	//logger->begin(debug_baudrate);
	//logger->println("InitSC16");
	pinMode(ss_pin, OUTPUT);
	//Set the transceiver to RS mode
	digitalWrite(ss_pin, LOW);
}

uint8_t RS485::processResponse(char*response,char first_char,char last_char,int response_size,int timeout)
{
	uint8_t pointer=0;
	int init_time;
	int time;
	memset(response,0,response_size); //Set to zeros the response (clean buffer)
	//Wait until bytes come from the
	init_time=millis();
	do
	{
		if(port->available()>0)
		{
			response[pointer]=port->read();//Read until initial character match
			logger->write(response[pointer]);
		}
		time=millis()-init_time;
	} while (response[pointer]!=first_char && time<timeout);
	logger->write("first matched");
	do 
	{
		if(port->available()>0)
		{
			response[pointer]=port->read();//Read until initial character match
			//logger->write(response[pointer]);
			pointer++;
		}
		time=millis()-init_time;
	} while (response[pointer-1]!=last_char && pointer<response_size && time<timeout);
	logger->write("Time:\n");
	logger->write(time);
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
	logger->write("Left on port:");
	logger->write(port->available());
}
void RS485::send(char*buffer,int length)
{
	int i;
	char result;
	//Enable Tx RS485
	digitalWrite(ss_pin,HIGH);
	//delayMicroseconds(100);//Waits until transmit holding register empties.
	//Waits until transmit holding register empties.
	for (i=0;i<length;i++)
	{
		port->write(buffer[i]);
	}
	while (!(UCSR0A & (1<<TXC0))){}
	//delayMicroseconds(400);//Waits until transmit holding register empties.
	//Enable Rx RS485
	digitalWrite(ss_pin,LOW);
}
void RS485::send(const PROGMEM char*buffer)
{
	char i=0;
	char result;
	//Enable Tx RS485
	digitalWrite(ss_pin,HIGH);
	do
	{
		port->write(pgm_read_byte(buffer+i));
		i++;
	}
	while(pgm_read_byte(buffer+i)!='\r');
	
	delayMicroseconds(400);//Waits until transmit holding register empties.
	//Enable Rx RS485
	digitalWrite(ss_pin,LOW);
}
