/*
 * timer_task.c
 *
 * Created: 17-08-2018 12:03:00
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
#define indicator_2 PORTC
#define IR_trigger_on (!(PINA & 0x01))

void timer_start();
void timer_stop();
void process();
void port_init();

int s_mode=0;
volatile int timer_count=0,IR_count=0,IR_trigger=0;

int main(void)
{
	sei();
	port_init();
		
    while (1) 
    {
		process();		
		indicator_1=IR_trigger;
		indicator_2=s_mode;
    }
}

void process()
{
 	timer_start();
	s_mode=0;
	if (IR_trigger==5)
	{
		s_mode=8;
	}
}

void port_init()
{
	DDRK=0xFF;				//indicator_1
	DDRC=0xFF;				//indicator_2
	DDRA=0x00;				//IR_input
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
	TCCR1A=0x00;
	TCCR1B=0x00;
	TIMSK1=0x00;	
	IR_count=0;
	IR_trigger=0;	
}

ISR(TIMER1_COMPA_vect)
{
	if (IR_trigger_on)
	{
		IR_count++;
	} 
	else
	{
		IR_count=0;
		IR_trigger=0;
	}
	if (IR_count>4)
	{
		IR_trigger=5;
	}
}