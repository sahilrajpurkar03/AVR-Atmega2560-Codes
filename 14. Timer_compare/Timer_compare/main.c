/*
 * Timer_compare.c
 *
 * Created: 11-08-2018 16:32:30
 * Author : Sahil
 */ 

#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define BAUD 9600									//serial com;
#define BAUDRATE  ((F_CPU/(BAUD*16UL)-1))

#define indicator_1 PORTK
#define IR_trigger_on (!(PINB & 0x01)

void timer_start();
void timer_stop();
void process();
void port_init();

int s_mode=0;
volatile int timer_count=0;

int main(void)
{
	sei();
	port_init();
		
    while (1) 
    {
		process();
//		indicator_1=s_mode;
    }
}

void process()
{
	timer_start();
	indicator_1=0xFF;
	if (timer_count %2==0)
	{
		indicator_1=0xAA;
	}
// 	if (IR_trigger_on)
// 	{
// 		timer_start();
// 		if (count<=)
// 		{
// 			timer_stop();
// 			s_mode=8;
// 		}
// 	}
}

void port_init()
{
	DDRK=0xFF;				//indicator_1
}

void timer_start()
{
	TCCR1A|=0x10;
	TCCR1B|=0x0D;
	TIMSK1|=0x02;
	OCR1A=14400;
}

void timer_stop()
{
	
}

ISR(TIMER1_COMPA_vect)
{
	timer_count++;
}