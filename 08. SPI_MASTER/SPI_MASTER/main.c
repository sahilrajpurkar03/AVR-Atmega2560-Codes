/*
 * SpI.c
 *
 * Created: 19-07-2018 21:01:04
 * Author : Sahil
 */ 

#define F_CPU 14745600UL 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define port_SPI PORTB
#define pin_MOSI PINB2
#define pin_SCK PINB1
#define pin_SS PINB0
#define indicator_1 PORTH

void port_init();
void SPI_MasterInit();
void SPI_MasterTransmit();

int transmit_value=6;

int main(void)
{
	port_init();
	SPI_MasterInit();
    while (1) 
    {	for (transmit_value=0;transmit_value<16;transmit_value++)
		{
				SPI_MasterTransmit(transmit_value);
				indicator_1=transmit_value;	
				_delay_ms(2000);
		}
	}
}

void port_init()
{
	DDRH=0xFF;			//indicator_1
	DDRB|=0x07;
	port_SPI|=(1<<pin_SS);	
}

void SPI_MasterInit()
{
	/* Set MOSI and SCK output, all others input */
	port_SPI =0x06;
	/* Enable SPI, Master, set clock rate fck/8 */
	SPCR = 0x51;
	SPSR = 0x01;
}
void SPI_MasterTransmit(int cData)
{
	port_SPI&=(~(1<<pin_SS));	
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	port_SPI|=(1<<pin_SS);
}
