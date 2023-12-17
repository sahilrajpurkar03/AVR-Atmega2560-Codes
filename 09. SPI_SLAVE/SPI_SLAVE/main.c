/*
 * SPI_SLAVE.c
 *
 * Created: 27-07-2018 18:56:16
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define port_SPI PORTB
#define pin_MISO PINB3
#define pin_SS PINB0
#define indicator_1 PORTH

void port_init();
void SPI_SlaveInit();
unsigned char SPI_SlaveReceive();

int main(void)
{
	port_init();
	SPI_SlaveInit();
	while (1)
	{
		indicator_1=SPI_SlaveReceive();
	}
}

void port_init()
{
	DDRH=0xFF;					//indicator_1
	DDRB|=0x03;					//SPI
	port_SPI |=(1<<pin_SS);
}

void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	port_SPI = (1<<pin_MISO);
	/* Enable SPI */
	SPCR = 0xC0;
}

unsigned char SPI_SlaveReceive(void)
{
	port_SPI&=(~(1<<pin_SS));
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))	;
	/* Return Data Register */
	return SPDR;
	port_SPI|=(1<<pin_SS);
}

