/*
 * RS485.h
 *
 * Created: 12/07/2014 11:48:46 a.m.
 *  Author: nedera03
 */ 

#ifndef RS485_H_
#define RS485_H_


#include "Print.h"
#include "DreadConfig.h"

enum SC_Status {
  SC_RESPOND_RECEIVED	=	0,
  SC_RESPOND_NOFORMAT,
  SC_RESPOND_TIMEOUT,
  SC_RESPOND_OUTOFBOUND   
};

class RS485 : public Print
{
	uint8_t write_pin;
  Print * port;
	public:
  void init(void);
	RS485(uint8_t const write_pin, Print & port);
  int read(void);
  void write(uint8_t c);
  void write(uint8_t const * buffer, uint16_t size);
  void flush(void);
  int available(void);
};

#endif /* RS485_H_ */
