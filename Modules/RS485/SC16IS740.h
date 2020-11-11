/*
 * SC16IS740.h
 *
 * Created: 05/03/2014 03:12:08 p.m.
 *  Author: nedera03
 */ 


#ifndef SC16IS740_H_
#define SC16IS740_H_
#include "Utilities/DreadSystem.h"
#include "Utilities/Print.h"
#include <avr/pgmspace.h>

//SC6IS740 Internal Registers //Note: all the registers are <<3
//General register set
#define RHR			0x00	//Read Original Reg: 0x00
#define THR			0x00	//Write Original Reg: 0x00
#define IER			0x08	//Read/Write Original Reg: 0x01
#define FCR			0x10	//Write Original Reg: 0x02
#define IIR			0x10	//Read Original Reg: 0x02
#define LCR			0x18	//Read/Write Original Reg: 0x03
#define MCR			0x20	//Read/Write *when EFR[4] is set Original Reg: 0x04
#define LSR			0x28	//Read Original Reg: 0x05
#define MSR			0x30	//Read Original Reg: 0x06
#define SPR			0x38	//Read/Write Original Reg: 0x07
#define TCR			0x30	//Read/Write *only when ERF[4] = 1 and MCR[2] = 1, Original Reg: 0x06
#define TLR			0x38	//Read/Write *only when ERF[4] = 1 and MCR[2] = 1, Original Reg: 0x07
#define TXLVL		0x40	//Read Original Reg: 0x08
#define RXLVL		0x48	//Read Original Reg: 0x09
#define IODir		0x50	//Read/Write Original Reg: 0x0A
#define IOState		0x58	//Read	*only on SC16IS750/SC16IS760 Original Reg: 0x0B
#define IOInteEna	0x60	//Read/Write *only on SC16IS750/SC16IS760 Original Reg: 0x0C
#define IOControl	0x70	//Read/Write *only on SC16IS750/SC16IS760 Original Reg: 0x0E
#define EFCR		0x78	//Read/Write *only on SC16IS750/SC16IS760 Original Reg: 0x0F
//Special register set
#define DLL		0x00	//Read/Write *only when LCR[7] is logic 1 Original Reg: 0x00
#define DLH		0x08	//Read/Write *only when LCR[7] is logic 1 Original Reg: 0x08
//Enhanced register set
#define EFR		0x10	//Read/Write *only when LCR is set to 1011 1111b (0xBF). Original Reg: 0x00
#define XON1	0x20	//Read/Write *only when LCR is set to 1011 1111b (0xBF).
#define XON2	0x05	//Read/Write *only when LCR is set to 1011 1111b (0xBF).
#define XOFF1	0x06	//Read/Write *only when LCR is set to 1011 1111b (0xBF).
#define XOFF2	0x07	//Read/Write *only when LCR is set to 1011 1111b (0xBF).
//Results
#define SC_RESPOND_TIMEOUT		2
#define SC_RESPOND_NOFORMAT		1
#define SC_RESPOND_RECEIVED		0
#define SC_RESPOND_OUTOFBOUND   3
//Driver handler
#define SC_12MHz		0x27
#define SC_1_8432MHz	0x06

class SC16IS740 : public Print
{
	uint8_t ss_pin;
	public:
	SC16IS740(uint8_t const ss_pin);
	void init(uint8_t const xtal_freq);
	void flush();
  int read(void);
  void write(uint8_t c);
  void write(uint8_t const * buffer, uint16_t size);
  int available(void);

  private:
	void write_SC16IS(uint8_t add,uint8_t val);
	uint8_t read_SC16IS(uint8_t add);
};

#endif /* SC16IS740_H_ */
