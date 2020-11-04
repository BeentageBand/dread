/*
 * RS485.h
 *
 * Created: 12/07/2014 11:48:46 a.m.
 *  Author: nedera03
 */ 

#ifndef RS485_H_
#define RS485_H_


#include "Utilities/Print.h"
#include <avr/pgmspace.h>

enum SC_Status {
  SC_RESPOND_RECEIVED	=	0,
  SC_RESPOND_NOFORMAT,
  SC_RESPOND_TIMEOUT,
  SC_RESPOND_OUTOFBOUND   
};

class RS485
{
	uint8_t ss_pin;
	Print * port;
	Print * logger;
	public:
	RS485(uint8_t const ss_pin, Print & port, Print & logger);
	void init(/*int debug_baudrate*/);
	void send(char*buffer, int length);
	void send(const PROGMEM char*buffer);
	uint8_t matchResponse(char*response, char first_char, char last_char,
      int response_size,
      uint8_t* res_ptr);
	uint8_t processResponse(char*response, char first_char, char last_char,
      int response_size,
      int timeout);
};

#endif /* RS485_H_ */
