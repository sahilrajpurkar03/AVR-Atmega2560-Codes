/*
 * DigitalRead.c
 *
 * Created: 13-01-2018 14:08:38
 * Author : Sahil
 */ 


#include <avr/io.h>

#define indicator_1 PORTA
#define indicator_2 PORTC

void digitalread_rising();
void digitalreal_falling();
void port_init();

int main(void)
{
	while (1) 
    {
		digitalread_rising();
		digitalreal_falling();
    }
}

void port_init()
{
	DDRB=0x00;				//Input port
	DDRD=0x00;				//Input port
	DDRA=0xFF;				//indicator_1
	DDRC=0xFF;				//indicator_2
}

void digitalread_rising()
{
	if(PINB & 0x01)
	{
		indicator_1=0x01;
	}
	else if(PINB & 0x02)
	{
		indicator_1=0x02;
	}										//Sens 1
	else if(PINB & 0x04)
	{
		indicator_1=0x04;
	}
	else
	{
		indicator_1=0x00;
	}
}

void digitalreal_falling()
{
	if(!(PIND & 0x01))
	{
		indicator_2=0x01;
	}
	else if(!(PIND & 0x02))
	{
		indicator_2=0x02;
	}									//Sens 0
	else if(!(PIND & 0x04))
	{
		indicator_2=0x04;
	}
	else
	{
		indicator_2=0x00;
	}

}